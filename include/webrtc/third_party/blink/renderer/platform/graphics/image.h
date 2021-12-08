/*
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2004, 2005, 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_GRAPHICS_IMAGE_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_GRAPHICS_IMAGE_H_

#include "base/macros.h"
#include "base/memory/scoped_refptr.h"
#include "base/memory/weak_ptr.h"
#include "third_party/blink/public/mojom/webpreferences/web_preferences.mojom-blink.h"
#include "third_party/blink/renderer/platform/geometry/float_point.h"
#include "third_party/blink/renderer/platform/geometry/float_size.h"
#include "third_party/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/blink/renderer/platform/graphics/graphics_types.h"
#include "third_party/blink/renderer/platform/graphics/image_observer.h"
#include "third_party/blink/renderer/platform/graphics/image_orientation.h"
#include "third_party/blink/renderer/platform/graphics/paint/paint_image.h"
#include "third_party/blink/renderer/platform/heap/persistent.h"
#include "third_party/blink/renderer/platform/platform_export.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"
#include "third_party/blink/renderer/platform/wtf/shared_buffer.h"
#include "third_party/blink/renderer/platform/wtf/thread_safe_ref_counted.h"
#include "third_party/skia/include/core/SkRefCnt.h"
#include "ui/base/resource/scale_factor.h"

class SkMatrix;

namespace cc {
class PaintCanvas;
class PaintFlags;
class ImageDecodeCache;
}  // namespace cc

namespace blink {

class FloatRect;
class GraphicsContext;
class Image;
class WebGraphicsContext3DProvider;
class WebGraphicsContext3DProviderWrapper;
class DarkModeImageCache;

class PLATFORM_EXPORT Image : public ThreadSafeRefCounted<Image> {
  friend class GeneratedImage;
  friend class CrossfadeGeneratedImage;
  friend class GradientGeneratedImage;
  friend class GraphicsContext;

 public:
  virtual ~Image();

  static cc::ImageDecodeCache& SharedCCDecodeCache(SkColorType);

  static scoped_refptr<Image> LoadPlatformResource(
      int resource_id,
      ui::ScaleFactor scale_factor = ui::SCALE_FACTOR_100P);

  static PaintImage ResizeAndOrientImage(
      const PaintImage&,
      ImageOrientation,
      FloatSize image_scale = FloatSize(1, 1),
      float opacity = 1.0,
      InterpolationQuality = kInterpolationNone);

  virtual bool IsSVGImage() const { return false; }
  virtual bool IsBitmapImage() const { return false; }
  virtual bool IsStaticBitmapImage() const { return false; }
  virtual bool IsPlaceholderImage() const { return false; }

  virtual bool CurrentFrameKnownToBeOpaque() = 0;

  virtual bool CurrentFrameIsComplete() { return false; }
  virtual bool CurrentFrameIsLazyDecoded() { return false; }
  virtual size_t FrameCount() { return 0; }
  virtual bool IsTextureBacked() const { return false; }

  // Derived classes should override this if they can assure that the current
  // image frame contains only resources from its own security origin.
  virtual bool CurrentFrameHasSingleSecurityOrigin() const { return false; }

  static Image* NullImage();
  bool IsNull() const { return Size().IsEmpty(); }

  virtual bool HasIntrinsicSize() const { return true; }

  virtual IntSize Size() const = 0;
  virtual IntSize DensityCorrectedSize() const { return Size(); }
  IntSize Size(RespectImageOrientationEnum) const;
  virtual IntSize PreferredDisplaySize() const { return Size(); }
  virtual FloatSize SizeAsFloat(
      RespectImageOrientationEnum respect_orientation) const {
    return FloatSize(Size(respect_orientation));
  }
  IntRect Rect() const { return IntRect(IntPoint(), Size()); }
  int width() const { return Size().Width(); }
  int height() const { return Size().Height(); }

  virtual bool GetHotSpot(IntPoint&) const { return false; }

  enum SizeAvailability {
    kSizeUnavailable,
    kSizeAvailableAndLoadingAsynchronously,
    kSizeAvailable,
  };

  // If SetData() returns |kSizeAvailableAndLoadingAsynchronously|:
  //   Image loading is continuing asynchronously
  //   (only when |this| is SVGImage and |all_data_received| is true), and
  //   ImageResourceObserver::AsyncLoadCompleted() is called when finished.
  // Otherwise:
  //   Image loading is completed synchronously.
  //   ImageResourceObserver::AsyncLoadCompleted() is not called.
  virtual SizeAvailability SetData(scoped_refptr<SharedBuffer> data,
                                   bool all_data_received);
  virtual SizeAvailability DataChanged(bool /*all_data_received*/) {
    return kSizeUnavailable;
  }

  // null string if unknown
  virtual String FilenameExtension() const;

  virtual void DestroyDecodedData() = 0;

  virtual scoped_refptr<SharedBuffer> Data() { return encoded_image_data_; }

  // Animation begins whenever someone draws the image, so startAnimation() is
  // not normally called. It will automatically pause once all observers no
  // longer want to render the image anywhere.
  virtual void StartAnimation() {}
  virtual void ResetAnimation() {}

  // True if this image can potentially animate.
  virtual bool MaybeAnimated() { return false; }

  // Set animationPolicy
  virtual void SetAnimationPolicy(mojom::blink::ImageAnimationPolicy) {}
  virtual mojom::blink::ImageAnimationPolicy AnimationPolicy() {
    return mojom::blink::ImageAnimationPolicy::kImageAnimationPolicyAllowed;
  }

  // Advances an animated image. For BitmapImage (e.g., animated gifs) this
  // will advance to the next frame. For SVGImage, this will trigger an
  // animation update for CSS and advance the SMIL timeline by one frame.
  virtual void AdvanceAnimationForTesting() {}

  // Typically the ImageResourceContent that owns us.
  ImageObserver* GetImageObserver() const {
    return image_observer_disabled_ ? nullptr : image_observer_;
  }
  void ClearImageObserver() { image_observer_ = nullptr; }
  // To avoid interleaved accesses to |m_imageObserverDisabled|, do not call
  // setImageObserverDisabled() other than from ImageObserverDisabler.
  void SetImageObserverDisabled(bool disabled) {
    image_observer_disabled_ = disabled;
  }

  virtual scoped_refptr<Image> ImageForDefaultFrame();

  enum ImageDecodingMode {
    // No preference specified.
    kUnspecifiedDecode,
    // Prefer to display the image synchronously with the rest of the content
    // updates.
    kSyncDecode,
    // Prefer to display the image asynchronously with the rest of the content
    // updates.
    kAsyncDecode
  };

  static PaintImage::DecodingMode ToPaintImageDecodingMode(
      ImageDecodingMode mode) {
    switch (mode) {
      case kUnspecifiedDecode:
        return PaintImage::DecodingMode::kUnspecified;
      case kSyncDecode:
        return PaintImage::DecodingMode::kSync;
      case kAsyncDecode:
        return PaintImage::DecodingMode::kAsync;
    }

    NOTREACHED();
    return PaintImage::DecodingMode::kUnspecified;
  }

  virtual PaintImage PaintImageForCurrentFrame() = 0;

  virtual bool HasDefaultOrientation() const { return true; }

  // Most image types have the default orientation. Only bitmap derived image
  // types need to override this method.
  virtual ImageOrientation CurrentFrameOrientation() const {
    return ImageOrientationEnum::kDefault;
  }

  virtual IntSize CurrentFrameDensityCorrectedSize() const { return IntSize(); }

  // Correct the src rect (rotate and maybe translate it) to account for a
  // non-default image orientation. The image must have non-default orientation
  // to call this method. The image_size is the oriented size of the image (i.e.
  // after orientation has been applied). src_rect may be a subset of the image,
  // also oriented.
  FloatRect CorrectSrcRectForImageOrientation(FloatSize image_size,
                                              FloatRect src_rect) const;

  enum ImageClampingMode {
    kClampImageToSourceRect,
    kDoNotClampImageToSourceRect
  };

  virtual void Draw(cc::PaintCanvas*,
                    const cc::PaintFlags&,
                    const FloatRect& dst_rect,
                    const FloatRect& src_rect,
                    RespectImageOrientationEnum,
                    ImageClampingMode,
                    ImageDecodingMode) = 0;

  virtual bool ApplyShader(cc::PaintFlags&, const SkMatrix& local_matrix);

  // Use ContextProvider() for immediate use only, use
  // ContextProviderWrapper() to obtain a retainable reference. Note:
  // Implemented only in sub-classes that use the GPU.
  virtual WebGraphicsContext3DProvider* ContextProvider() const {
    return nullptr;
  }
  virtual base::WeakPtr<WebGraphicsContext3DProviderWrapper>
  ContextProviderWrapper() const {
    return nullptr;
  }

  PaintImage::Id paint_image_id() const { return stable_image_id_; }

  // Returns an SkBitmap that is a copy of the image's current frame.
  SkBitmap AsSkBitmapForCurrentFrame(RespectImageOrientationEnum);

  DarkModeImageCache* GetDarkModeImageCache();

 protected:
  Image(ImageObserver* = nullptr, bool is_multipart = false);

  virtual void DrawPattern(GraphicsContext&,
                           const FloatRect&,
                           const FloatSize&,
                           const FloatPoint& phase,
                           SkBlendMode,
                           const FloatRect&,
                           const FloatSize& repeat_spacing,
                           RespectImageOrientationEnum);

  // Creates and initializes a PaintImageBuilder with the metadata flags for the
  // PaintImage.
  PaintImageBuilder CreatePaintImageBuilder();

  // Whether or not size is available yet.
  virtual bool IsSizeAvailable() { return true; }

 private:
  bool image_observer_disabled_;
  scoped_refptr<SharedBuffer> encoded_image_data_;
  // TODO(Oilpan): consider having Image on the Oilpan heap and
  // turn this into a Member<>.
  //
  // The observer (an ImageResourceContent) is responsible for clearing
  // itself out when it switches to another Image.
  // When the ImageResourceContent is garbage collected while Image is still
  // alive, |image_observer_| is cleared by WeakPersistent mechanism.
  WeakPersistent<ImageObserver> image_observer_;
  PaintImage::Id stable_image_id_;
  const bool is_multipart_;
  std::unique_ptr<DarkModeImageCache> dark_mode_image_cache_;
  DISALLOW_COPY_AND_ASSIGN(Image);
};

}  // namespace blink

#endif
