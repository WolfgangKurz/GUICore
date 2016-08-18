/*
	GUICore - GUIObject
	 The win32 GUI framework library.
	 author: WolfgangKurz

	Base class for windows and widgets
	A standard object unit of system
*/

#include "GUICore.hpp"
#include "GUIObject.hpp"
#include "GUIValues.h"

#define GUIObjectClass  L"GUIObject"
#define GUIShadowObjectClass  L"GUIShadowObject"

GUIObject::GUIObject(UINT flag, UINT style, UINT exstyle, HWND parent){
	HINSTANCE hInst = GetModuleHandle((LPCWSTR)NULL);

	wchar_t *className = GUIObjectClass;
	if((flag&GUIOF_SHADOW)>0) className = GUIShadowObjectClass;

	style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	this->hWnd = CreateWindowEx(exstyle, className, L"GUIObject", style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, (HMENU)NULL, hInst, NULL);
	if( this->hWnd==(HWND)NULL ) return;

	this->hFont = (HFONT)NULL;
	this->parent = (GUIObject*)NULL;
	this->extra = (void*)NULL;
	this->objFlag = flag;
	this->onMessage = (GUIMessageEventP)NULL;

	SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)((void*)this));
}
GUIObject::GUIObject(int x, int y, int width, int height, UINT flag, UINT style, UINT exstyle, HWND parent){
	HINSTANCE hInst = GetModuleHandle((LPCWSTR)NULL);

	wchar_t *className = GUIObjectClass;
	if((flag&GUIOF_SHADOW)>0) className = GUIShadowObjectClass;

	if((flag&GUIOF_ADJUST)>0){
		RECT rc = {0, 0, width, height};
		AdjustWindowRect(&rc, style, FALSE);
		width = rc.right-rc.left;
		height = rc.bottom-rc.top;
	}

	style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	this->hWnd = CreateWindowEx(exstyle, className, L"GUIObject", style, x, y, width, height, parent, (HMENU)NULL, hInst, NULL);
	if( this->hWnd==(HWND)NULL ) return;

	this->hFont = (HFONT)NULL;
	this->parent = (GUIObject*)NULL;
	this->extra = (void*)NULL;
	this->objFlag = flag;
	this->onMessage = (GUIMessageEventP)NULL;

	SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)((void*)this));
}
GUIObject::~GUIObject(){
	if(this->extra!=(void*)NULL){
		delete this->extra;
	}
	if(this->hFont!=(HFONT)NULL){
		DeleteObject(this->hFont);
		this->hFont = (HFONT)NULL;
	}
	if( this->hWnd!=(HWND)NULL ){
		DestroyWindow(this->hWnd);
		this->hWnd = (HWND)NULL;
	}
	if(this->onMessage){ delete this->onMessage; this->onMessage = NULL; }
}

HWND GUIObject::getHWND(){
	return this->hWnd;
}
GUIObject* GUIObject::setParent(GUIObject *parent){
	this->parent = parent;
	SetParent(this->hWnd, parent->getHWND());
	return this;
}
GUIObject* GUIObject::getParent(){
	return this->parent;
}

GUIObject* GUIObject::setFont(const wchar_t *fontName, int fontSize, int weight){
	this->hFont = CreateFont(-fontSize, 0, 0, 0, weight, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, fontName);
	this->invalidate();
	return this;
}
GUIObject* GUIObject::setFont(HFONT font){
	this->hFont = font;
	this->invalidate();
	return this;
}
HFONT GUIObject::getFont(){
	return this->hFont;
}

GUIObject* GUIObject::invalidate(BOOL immediately){
	if( this->hWnd!=(HWND)NULL ){
		InvalidateRect(this->hWnd, NULL, FALSE);
		if(immediately) UpdateWindow(this->hWnd);
	}
	return this;
}

GUIObject* GUIObject::show(int show){
	if( this->hWnd!=(HWND)NULL ){
		ShowWindow(this->hWnd, show);
	}
	return this;
}
GUIObject* GUIObject::hide(){
	if( this->hWnd!=(HWND)NULL ){
		ShowWindow(this->hWnd, SW_HIDE);
	}
	return this;
}

GUIObject* GUIObject::move_center(){
	if( this->hWnd!=(HWND)NULL ){
		RECT rc, rct;
		HWND parent = GetParent(this->hWnd);

		if( parent==(HWND)NULL ){
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rct, 0);
		}else{
			GetWindowRect(parent, &rct);
		}

		GetWindowRect(this->hWnd, &rc);
		SetWindowPos(this->hWnd, HWND_TOP, (rct.right-rct.left)/2-(rc.right-rc.left)/2, (rct.bottom-rct.top)/2-(rc.bottom-rc.top)/2, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
	}
	return this;
}
GUIObject* GUIObject::move(int x, int y){
	if( this->hWnd!=(HWND)NULL ){
		SetWindowPos(this->hWnd, (HWND)NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
	}
	return this;
}
GUIObject* GUIObject::resize(int width, int height){
	if( this->hWnd!=(HWND)NULL ){
		if((this->objFlag|GUIOF_ADJUST)>0){
			RECT rc = {0, 0, width, height};
			AdjustWindowRect(&rc, GetWindowLong(this->hWnd, GWL_STYLE), GetMenu(this->hWnd)!=(HMENU)NULL ? TRUE : FALSE);
			width = rc.right-rc.left;
			height = rc.bottom-rc.top;
		}

		SetWindowPos(this->hWnd, (HWND)NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
	}
	return this;
}

int GUIObject::getX(){
	if( this->hWnd!=(HWND)NULL ){
		RECT rc;
		GetWindowRect(this->hWnd, &rc);
		return rc.left;
	}
	return 0;
}
int GUIObject::getY(){
	if( this->hWnd!=(HWND)NULL ){
		RECT rc;
		GetWindowRect(this->hWnd, &rc);
		return rc.top;
	}
	return 0;
}
int GUIObject::getWidth(){
	if( this->hWnd!=(HWND)NULL ){
		RECT rc;
		GetWindowRect(this->hWnd, &rc);
		return rc.right-rc.left;
	}
	return -1;
}
int GUIObject::getHeight(){
	if( this->hWnd!=(HWND)NULL ){
		RECT rc;
		GetWindowRect(this->hWnd, &rc);
		return rc.bottom-rc.top;
	}
	return -1;
}
int GUIObject::getInnerWidth(){
	if( this->hWnd!=(HWND)NULL ){
		RECT rc;
		GetClientRect(this->hWnd, &rc);
		return rc.right-rc.left;
	}
	return -1;
}
int GUIObject::getInnerHeight(){
	if( this->hWnd!=(HWND)NULL ){
		RECT rc;
		GetClientRect(this->hWnd, &rc);
		return rc.bottom-rc.top;
	}
	return -1;
}

LRESULT GUIObject::handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	LRESULT ret = (LRESULT)NULL;
	
	if(this->onMessage){
		if(((GUIMessageEvent*)this->onMessage)->Event(this, Msg, wParam, lParam, &ret)==FALSE){
			ret = DefWindowProc(hWnd, Msg, wParam, lParam);
		}else{
			return ret;
		}
	}else{
		ret = DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return ret;
}

GUIObject* GUIObject::setOnMessage(void *onMessage){
	if(this->onMessage) delete this->onMessage;
	this->onMessage = onMessage;
	return this;
}
void* GUIObject::getOnMessage(){
	return this->onMessage;
}
