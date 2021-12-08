#pragma once
#ifdef WIN32
#include <SDKDDKVer.h>
#include <windows.h>
#endif

template <typename T>
class AutoLock {
public:
	explicit AutoLock(T* obj) : obj_(obj) { obj_->Lock(); }
	~AutoLock() { obj_->Unlock(); }

protected:
	T* obj_;
};
