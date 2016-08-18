/*
	GUICore - GUICore
	 The win32 GUI framework library.
	 author: WolfgangKurz

	Core class
	Contains many static utility methods
*/

#include "GUICore.hpp"
#include "GUIValues.h"

#include <stdio.h>

#define GUIObjectClass  L"GUIObject"
#define GUIShadowObjectClass  L"GUIShadowObject"

LRESULT WINAPI GUICoreProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


GUICore::GUICore() {
	/// TODO
}
GUICore::~GUICore() {
	/// TODO
}

void GUICore::initialize(){
	HINSTANCE hInst = GetModuleHandle(NULL);
	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInst;
	wc.lpfnWndProc = GUICoreProc;
	wc.lpszClassName = GUIObjectClass;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	wc.lpszClassName = GUIShadowObjectClass;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW;
	RegisterClass(&wc);
}
void GUICore::uninitialize(){
	HINSTANCE hInst = GetModuleHandle(NULL);
	UnregisterClass(GUIShadowObjectClass, hInst);
	UnregisterClass(GUIObjectClass, hInst);
}

void GUICore::consoleHide(){
	HWND console = GetConsoleWindow();
	if( console==NULL ) return;
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}
void GUICore::consoleShow(){
	HWND console = GetConsoleWindow();
	if( console==NULL ){
		FILE *stream = (FILE*)NULL;
		AllocConsole();

		freopen_s(&stream, "CONIN$", "r", stdin); 
		freopen_s(&stream, "CONOUT$", "w", stdout);
		freopen_s(&stream, "CONOUT$", "w", stderr);
	}else{
		ShowWindow(GetConsoleWindow(), SW_SHOW);
	}
}

GUIObject* GUICore::rootObject(GUIObject *object){
	GUIObject *out = object;
	while( out->getParent()!=(GUIObject*)NULL )
		out = out->getParent();
	return out;
}
GUIWindow* GUICore::parentWindow(GUIObject *object){
	return (GUIWindow*)GUICore::rootObject(object);
}

int GUICore::loop(BOOL peek, HACCEL accel){
	MSG msg;
	if( accel==(HACCEL)NULL ){
		if(peek==TRUE){
			while( PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE) ){
				if( msg.message==WM_QUIT ) break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}else{
			while( GetMessage(&msg, (HWND)NULL, 0, 0) ){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}else{
		if(peek==TRUE){
			while( PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE) ){
				if( !TranslateAccelerator(msg.hwnd, accel, &msg) ){
					if( msg.message==WM_QUIT ) break;
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}else{
			while( GetMessage(&msg, (HWND)NULL, 0, 0) ){
				if( !TranslateAccelerator(msg.hwnd, accel, &msg) ){
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			} // while GetMessage
		} // peek==TRUE
	} // accel==NULL
	return (int)msg.wParam;
}
int GUICore::quit(){
	PostQuitMessage(0);
	return 0;
}

LRESULT WINAPI GUICoreProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	GUIObject *obj = (GUIObject*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if( obj!=(GUIObject*)NULL ){
		return obj->handleMessage(hWnd, Msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
