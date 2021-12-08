// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: network_tester_packet.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_network_5ftester_5fpacket_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_network_5ftester_5fpacket_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3013000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3013000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_util.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_network_5ftester_5fpacket_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_network_5ftester_5fpacket_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
namespace webrtc {
namespace network_tester {
namespace packet {
class NetworkTesterPacket;
class NetworkTesterPacketDefaultTypeInternal;
extern NetworkTesterPacketDefaultTypeInternal _NetworkTesterPacket_default_instance_;
}  // namespace packet
}  // namespace network_tester
}  // namespace webrtc
PROTOBUF_NAMESPACE_OPEN
template<> ::webrtc::network_tester::packet::NetworkTesterPacket* Arena::CreateMaybeMessage<::webrtc::network_tester::packet::NetworkTesterPacket>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace webrtc {
namespace network_tester {
namespace packet {

enum NetworkTesterPacket_Type : int {
  NetworkTesterPacket_Type_HAND_SHAKING = 0,
  NetworkTesterPacket_Type_TEST_START = 1,
  NetworkTesterPacket_Type_TEST_DATA = 2,
  NetworkTesterPacket_Type_TEST_DONE = 3
};
bool NetworkTesterPacket_Type_IsValid(int value);
constexpr NetworkTesterPacket_Type NetworkTesterPacket_Type_Type_MIN = NetworkTesterPacket_Type_HAND_SHAKING;
constexpr NetworkTesterPacket_Type NetworkTesterPacket_Type_Type_MAX = NetworkTesterPacket_Type_TEST_DONE;
constexpr int NetworkTesterPacket_Type_Type_ARRAYSIZE = NetworkTesterPacket_Type_Type_MAX + 1;

const std::string& NetworkTesterPacket_Type_Name(NetworkTesterPacket_Type value);
template<typename T>
inline const std::string& NetworkTesterPacket_Type_Name(T enum_t_value) {
  static_assert(::std::is_same<T, NetworkTesterPacket_Type>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function NetworkTesterPacket_Type_Name.");
  return NetworkTesterPacket_Type_Name(static_cast<NetworkTesterPacket_Type>(enum_t_value));
}
bool NetworkTesterPacket_Type_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, NetworkTesterPacket_Type* value);
// ===================================================================

class NetworkTesterPacket PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::MessageLite /* @@protoc_insertion_point(class_definition:webrtc.network_tester.packet.NetworkTesterPacket) */ {
 public:
  inline NetworkTesterPacket() : NetworkTesterPacket(nullptr) {}
  virtual ~NetworkTesterPacket();

  NetworkTesterPacket(const NetworkTesterPacket& from);
  NetworkTesterPacket(NetworkTesterPacket&& from) noexcept
    : NetworkTesterPacket() {
    *this = ::std::move(from);
  }

  inline NetworkTesterPacket& operator=(const NetworkTesterPacket& from) {
    CopyFrom(from);
    return *this;
  }
  inline NetworkTesterPacket& operator=(NetworkTesterPacket&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const std::string& unknown_fields() const {
    return _internal_metadata_.unknown_fields<std::string>(::PROTOBUF_NAMESPACE_ID::internal::GetEmptyString);
  }
  inline std::string* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<std::string>();
  }

  static const NetworkTesterPacket& default_instance();

  static inline const NetworkTesterPacket* internal_default_instance() {
    return reinterpret_cast<const NetworkTesterPacket*>(
               &_NetworkTesterPacket_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(NetworkTesterPacket& a, NetworkTesterPacket& b) {
    a.Swap(&b);
  }
  PROTOBUF_NOINLINE void Swap(NetworkTesterPacket* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(NetworkTesterPacket* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline NetworkTesterPacket* New() const final {
    return CreateMaybeMessage<NetworkTesterPacket>(nullptr);
  }

  NetworkTesterPacket* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<NetworkTesterPacket>(arena);
  }
  void CheckTypeAndMergeFrom(const ::PROTOBUF_NAMESPACE_ID::MessageLite& from)
    final;
  void CopyFrom(const NetworkTesterPacket& from);
  void MergeFrom(const NetworkTesterPacket& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  void DiscardUnknownFields();
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(NetworkTesterPacket* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "webrtc.network_tester.packet.NetworkTesterPacket";
  }
  protected:
  explicit NetworkTesterPacket(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  std::string GetTypeName() const final;

  // nested types ----------------------------------------------------

  typedef NetworkTesterPacket_Type Type;
  static constexpr Type HAND_SHAKING =
    NetworkTesterPacket_Type_HAND_SHAKING;
  static constexpr Type TEST_START =
    NetworkTesterPacket_Type_TEST_START;
  static constexpr Type TEST_DATA =
    NetworkTesterPacket_Type_TEST_DATA;
  static constexpr Type TEST_DONE =
    NetworkTesterPacket_Type_TEST_DONE;
  static inline bool Type_IsValid(int value) {
    return NetworkTesterPacket_Type_IsValid(value);
  }
  static constexpr Type Type_MIN =
    NetworkTesterPacket_Type_Type_MIN;
  static constexpr Type Type_MAX =
    NetworkTesterPacket_Type_Type_MAX;
  static constexpr int Type_ARRAYSIZE =
    NetworkTesterPacket_Type_Type_ARRAYSIZE;
  template<typename T>
  static inline const std::string& Type_Name(T enum_t_value) {
    static_assert(::std::is_same<T, Type>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function Type_Name.");
    return NetworkTesterPacket_Type_Name(enum_t_value);
  }
  static inline bool Type_Parse(::PROTOBUF_NAMESPACE_ID::ConstStringParam name,
      Type* value) {
    return NetworkTesterPacket_Type_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kSendTimestampFieldNumber = 2,
    kArrivalTimestampFieldNumber = 3,
    kTypeFieldNumber = 1,
    kPacketSizeFieldNumber = 5,
    kSequenceNumberFieldNumber = 4,
  };
  // optional int64 send_timestamp = 2;
  bool has_send_timestamp() const;
  private:
  bool _internal_has_send_timestamp() const;
  public:
  void clear_send_timestamp();
  ::PROTOBUF_NAMESPACE_ID::int64 send_timestamp() const;
  void set_send_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_send_timestamp() const;
  void _internal_set_send_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // optional int64 arrival_timestamp = 3;
  bool has_arrival_timestamp() const;
  private:
  bool _internal_has_arrival_timestamp() const;
  public:
  void clear_arrival_timestamp();
  ::PROTOBUF_NAMESPACE_ID::int64 arrival_timestamp() const;
  void set_arrival_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_arrival_timestamp() const;
  void _internal_set_arrival_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // optional .webrtc.network_tester.packet.NetworkTesterPacket.Type type = 1;
  bool has_type() const;
  private:
  bool _internal_has_type() const;
  public:
  void clear_type();
  ::webrtc::network_tester::packet::NetworkTesterPacket_Type type() const;
  void set_type(::webrtc::network_tester::packet::NetworkTesterPacket_Type value);
  private:
  ::webrtc::network_tester::packet::NetworkTesterPacket_Type _internal_type() const;
  void _internal_set_type(::webrtc::network_tester::packet::NetworkTesterPacket_Type value);
  public:

  // optional int32 packet_size = 5;
  bool has_packet_size() const;
  private:
  bool _internal_has_packet_size() const;
  public:
  void clear_packet_size();
  ::PROTOBUF_NAMESPACE_ID::int32 packet_size() const;
  void set_packet_size(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_packet_size() const;
  void _internal_set_packet_size(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // optional int64 sequence_number = 4;
  bool has_sequence_number() const;
  private:
  bool _internal_has_sequence_number() const;
  public:
  void clear_sequence_number();
  ::PROTOBUF_NAMESPACE_ID::int64 sequence_number() const;
  void set_sequence_number(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_sequence_number() const;
  void _internal_set_sequence_number(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // @@protoc_insertion_point(class_scope:webrtc.network_tester.packet.NetworkTesterPacket)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::int64 send_timestamp_;
  ::PROTOBUF_NAMESPACE_ID::int64 arrival_timestamp_;
  int type_;
  ::PROTOBUF_NAMESPACE_ID::int32 packet_size_;
  ::PROTOBUF_NAMESPACE_ID::int64 sequence_number_;
  friend struct ::TableStruct_network_5ftester_5fpacket_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// NetworkTesterPacket

// optional .webrtc.network_tester.packet.NetworkTesterPacket.Type type = 1;
inline bool NetworkTesterPacket::_internal_has_type() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool NetworkTesterPacket::has_type() const {
  return _internal_has_type();
}
inline void NetworkTesterPacket::clear_type() {
  type_ = 0;
  _has_bits_[0] &= ~0x00000004u;
}
inline ::webrtc::network_tester::packet::NetworkTesterPacket_Type NetworkTesterPacket::_internal_type() const {
  return static_cast< ::webrtc::network_tester::packet::NetworkTesterPacket_Type >(type_);
}
inline ::webrtc::network_tester::packet::NetworkTesterPacket_Type NetworkTesterPacket::type() const {
  // @@protoc_insertion_point(field_get:webrtc.network_tester.packet.NetworkTesterPacket.type)
  return _internal_type();
}
inline void NetworkTesterPacket::_internal_set_type(::webrtc::network_tester::packet::NetworkTesterPacket_Type value) {
  assert(::webrtc::network_tester::packet::NetworkTesterPacket_Type_IsValid(value));
  _has_bits_[0] |= 0x00000004u;
  type_ = value;
}
inline void NetworkTesterPacket::set_type(::webrtc::network_tester::packet::NetworkTesterPacket_Type value) {
  _internal_set_type(value);
  // @@protoc_insertion_point(field_set:webrtc.network_tester.packet.NetworkTesterPacket.type)
}

// optional int64 send_timestamp = 2;
inline bool NetworkTesterPacket::_internal_has_send_timestamp() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool NetworkTesterPacket::has_send_timestamp() const {
  return _internal_has_send_timestamp();
}
inline void NetworkTesterPacket::clear_send_timestamp() {
  send_timestamp_ = PROTOBUF_LONGLONG(0);
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 NetworkTesterPacket::_internal_send_timestamp() const {
  return send_timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 NetworkTesterPacket::send_timestamp() const {
  // @@protoc_insertion_point(field_get:webrtc.network_tester.packet.NetworkTesterPacket.send_timestamp)
  return _internal_send_timestamp();
}
inline void NetworkTesterPacket::_internal_set_send_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _has_bits_[0] |= 0x00000001u;
  send_timestamp_ = value;
}
inline void NetworkTesterPacket::set_send_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_send_timestamp(value);
  // @@protoc_insertion_point(field_set:webrtc.network_tester.packet.NetworkTesterPacket.send_timestamp)
}

// optional int64 arrival_timestamp = 3;
inline bool NetworkTesterPacket::_internal_has_arrival_timestamp() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool NetworkTesterPacket::has_arrival_timestamp() const {
  return _internal_has_arrival_timestamp();
}
inline void NetworkTesterPacket::clear_arrival_timestamp() {
  arrival_timestamp_ = PROTOBUF_LONGLONG(0);
  _has_bits_[0] &= ~0x00000002u;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 NetworkTesterPacket::_internal_arrival_timestamp() const {
  return arrival_timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 NetworkTesterPacket::arrival_timestamp() const {
  // @@protoc_insertion_point(field_get:webrtc.network_tester.packet.NetworkTesterPacket.arrival_timestamp)
  return _internal_arrival_timestamp();
}
inline void NetworkTesterPacket::_internal_set_arrival_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _has_bits_[0] |= 0x00000002u;
  arrival_timestamp_ = value;
}
inline void NetworkTesterPacket::set_arrival_timestamp(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_arrival_timestamp(value);
  // @@protoc_insertion_point(field_set:webrtc.network_tester.packet.NetworkTesterPacket.arrival_timestamp)
}

// optional int64 sequence_number = 4;
inline bool NetworkTesterPacket::_internal_has_sequence_number() const {
  bool value = (_has_bits_[0] & 0x00000010u) != 0;
  return value;
}
inline bool NetworkTesterPacket::has_sequence_number() const {
  return _internal_has_sequence_number();
}
inline void NetworkTesterPacket::clear_sequence_number() {
  sequence_number_ = PROTOBUF_LONGLONG(0);
  _has_bits_[0] &= ~0x00000010u;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 NetworkTesterPacket::_internal_sequence_number() const {
  return sequence_number_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 NetworkTesterPacket::sequence_number() const {
  // @@protoc_insertion_point(field_get:webrtc.network_tester.packet.NetworkTesterPacket.sequence_number)
  return _internal_sequence_number();
}
inline void NetworkTesterPacket::_internal_set_sequence_number(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _has_bits_[0] |= 0x00000010u;
  sequence_number_ = value;
}
inline void NetworkTesterPacket::set_sequence_number(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_sequence_number(value);
  // @@protoc_insertion_point(field_set:webrtc.network_tester.packet.NetworkTesterPacket.sequence_number)
}

// optional int32 packet_size = 5;
inline bool NetworkTesterPacket::_internal_has_packet_size() const {
  bool value = (_has_bits_[0] & 0x00000008u) != 0;
  return value;
}
inline bool NetworkTesterPacket::has_packet_size() const {
  return _internal_has_packet_size();
}
inline void NetworkTesterPacket::clear_packet_size() {
  packet_size_ = 0;
  _has_bits_[0] &= ~0x00000008u;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 NetworkTesterPacket::_internal_packet_size() const {
  return packet_size_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 NetworkTesterPacket::packet_size() const {
  // @@protoc_insertion_point(field_get:webrtc.network_tester.packet.NetworkTesterPacket.packet_size)
  return _internal_packet_size();
}
inline void NetworkTesterPacket::_internal_set_packet_size(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _has_bits_[0] |= 0x00000008u;
  packet_size_ = value;
}
inline void NetworkTesterPacket::set_packet_size(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_packet_size(value);
  // @@protoc_insertion_point(field_set:webrtc.network_tester.packet.NetworkTesterPacket.packet_size)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace packet
}  // namespace network_tester
}  // namespace webrtc

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::webrtc::network_tester::packet::NetworkTesterPacket_Type> : ::std::true_type {};

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_network_5ftester_5fpacket_2eproto
