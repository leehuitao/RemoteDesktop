#pragma once

#include <memory>
#include "common.h"

typedef intptr_t CapSourceID;

struct CAPString {
	char* buff;
	size_t length;
};

struct CAPSourceList
{
	CapSourceID  sourceid;
	CAPString	title;
};

CAPString* MallocCapString(int len);

void ReleaseCapString(CAPString* str);

enum CapStaus
{
	kSigaConncting,
	kSigaConnectd,
	kSigaDissolution,
	kConnecting,
	kConnected,
	kOnRender,
	kDisconnected
};

enum CapError {
	ERROR_SIGNA_CONNECT_FAILD,
	ERROR_ICE_CONNECT_FAILD,
	ERROR_ICE_ERROR
};

struct cb_struct
{
	void* cb_;
	void* param;
};
//call back
typedef void(*cb_on_status)(void* param,int);
typedef void(*cb_onerror)(void* param, int);
typedef void(*cb_onimage)(void* param, char* image, int width, int height, int bitcount);
typedef void(*cb_ondata)(void* param, char* data, int len);

#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) bool init_capture_sdk(CAPString& signasessionid);
	__declspec(dllexport) bool destory_capture_sdk();
	__declspec(dllexport) void set_signa_server(CAPString& ip, int port);
	__declspec(dllexport) void add_ice_server(CAPString& svrurl);
	__declspec(dllexport) void add_ice_server_with_name(CAPString& svrurl, CAPString& user, CAPString& pwd);

	__declspec(dllexport) int get_monitors(CAPSourceList* mlist);
	__declspec(dllexport) int get_windows(CAPSourceList* wlist);

	__declspec(dllexport) bool start_monitors_cap(CapSourceID id);
	__declspec(dllexport) bool start_window_cap(CapSourceID id);
	__declspec(dllexport) void stop_cap();
	__declspec(dllexport) bool suspend();
	__declspec(dllexport) bool send_data(char* data, int len);
	__declspec(dllexport) bool start_render();

	__declspec(dllexport) void Set_Status_cb(void* param, cb_on_status cb);
	__declspec(dllexport) void Set_Error_cb(void* param, cb_onerror cb);
	__declspec(dllexport) void Set_image_cb(void* param, cb_onimage cb);
	__declspec(dllexport) void Set_Data_cb(void* param, cb_ondata cb);
#ifdef __cplusplus
}
#endif

