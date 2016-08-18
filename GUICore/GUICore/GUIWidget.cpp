/*
	GUICore - GUIWidget
	 The win32 GUI framework library.
	 author: WolfgangKurz

	Base class for widgets
	A standard unit of child controls
*/

#include "GUIWidget.hpp"
#include "GUIValues.h"

GUIWidget::GUIWidget(int x, int y, int width, int height, UINT style, UINT exstyle) : GUIObject(x, y, width, height, GUIOF_NONE, WS_CHILD | WS_TABSTOP | style, exstyle, GetDesktopWindow()){
	this->onUnload      = (GUIEventP)NULL;
	this->onFocus       = (GUIFocusEventP)NULL;
	this->onKillFocus   = (GUIFocusEventP)NULL;
	this->onClick       = (GUIMouseEventP)NULL;
	this->onDblClick    = (GUIMouseEventP)NULL;
	this->onMouseDown   = (GUIMouseEventP)NULL;
	this->onMouseMove   = (GUIMouseEventP)NULL;
	this->onMouseUp     = (GUIMouseEventP)NULL;
	this->onMouseHover  = (GUIEventP)NULL;
	this->onMouseLeave  = (GUIEventP)NULL;
	this->onKeyDown     = (GUIKeyEventP)NULL;
	this->onKeyPress    = (GUIKeyEventP)NULL;
	this->onKeyUp       = (GUIKeyEventP)NULL;
	this->onPaint       = (GUIPaintEventP)NULL;
	this->onMove        = (GUIMoveEventP)NULL;
	this->onResize      = (GUIResizeEventP)NULL;
}
GUIWidget::~GUIWidget(){
	if(this->onUnload)    { delete this->onUnload;     this->onUnload=NULL; }
	if(this->onFocus)     { delete this->onFocus;      this->onFocus=NULL; }
	if(this->onKillFocus) { delete this->onKillFocus;  this->onKillFocus=NULL; }
	if(this->onClick)     { delete this->onClick;      this->onClick=NULL; }
	if(this->onDblClick)  { delete this->onDblClick;   this->onDblClick=NULL; }
	if(this->onMouseDown) { delete this->onMouseDown;  this->onMouseDown=NULL; }
	if(this->onMouseMove) { delete this->onMouseMove;  this->onMouseMove=NULL; }
	if(this->onMouseUp)   { delete this->onMouseUp;    this->onMouseUp=NULL; }
	if(this->onMouseHover){ delete this->onMouseHover; this->onMouseHover=NULL; }
	if(this->onMouseLeave){ delete this->onMouseLeave; this->onMouseLeave=NULL; }
	if(this->onKeyDown)   { delete this->onKeyDown;    this->onKeyDown=NULL; }
	if(this->onKeyPress)  { delete this->onKeyPress;   this->onKeyPress=NULL; }
	if(this->onKeyUp)     { delete this->onKeyUp;      this->onKeyUp=NULL; }
	if(this->onPaint)     { delete this->onPaint;      this->onPaint=NULL; }
	if(this->onMove)      { delete this->onMove;       this->onMove=NULL; }
	if(this->onResize)    { delete this->onResize;     this->onResize=NULL; }
}

GUIWidget* GUIWidget::setParent(GUIObject *parent){
	GUIObject::setParent(parent);
	return this;
}

GUIWidget* GUIWidget::setFont(const wchar_t *fontName, int fontSize, int weight){
	GUIObject::setFont(fontName, fontSize, weight);
	return this;
}
GUIWidget* GUIWidget::setFont(HFONT font){
	GUIObject::setFont(font);
	return this;
}

GUIWidget* GUIWidget::invalidate(BOOL immediately){
	GUIObject::invalidate(immediately);
	return this;
}

GUIWidget* GUIWidget::show(int show){
	GUIObject::show(show);
	return this;
}
GUIWidget* GUIWidget::hide(){
	GUIObject::hide();
	return this;
}

GUIWidget* GUIWidget::move_center(){
	GUIObject::move_center();
	return this;
}
GUIWidget* GUIWidget::move(int x, int y){
	GUIObject::move(x, y);
	return this;
}
GUIWidget* GUIWidget::resize(int width, int height){
	GUIObject::resize(width, height);
	return this;
}

LRESULT GUIWidget::handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	switch(Msg){
		case WM_DESTROY:
			if(this->onUnload) this->onUnload->Event(this);

		case WM_MOUSEMOVE:
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_HOVER | TME_LEAVE;
				tme.hwndTrack = this->hWnd;
				TrackMouseEvent(&tme);
			}

			if(this->onMouseMove)
				this->onMouseMove->Event(this,
					(GUIMouse)(((wParam&MK_LBUTTON)==MK_LBUTTON ? GUIMouseLeft : 0) | ((wParam&MK_RBUTTON)==MK_RBUTTON ? GUIMouseRight : 0) | ((wParam&MK_MBUTTON)==MK_MBUTTON ? GUIMouseMiddle : 0)),
					(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
					LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONDBLCLK:
			if(this->onDblClick)
				this->onDblClick->Event(this, GUIMouseLeft,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONDOWN:
			if(this->onMouseDown)
				this->onMouseDown->Event(this, GUIMouseLeft,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONUP:
			if(this->onMouseUp){
				if( this->onMouseUp->Event(this, GUIMouseLeft,
					(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
					LOWORD(lParam), HIWORD(lParam))!=0 )
					break;
			}
			if(this->onClick) this->onClick->Event(this, GUIMouseLeft,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_RBUTTONDBLCLK:
			if(this->onDblClick)
				this->onDblClick->Event(this, GUIMouseRight,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_RBUTTONDOWN:
			if(this->onMouseDown)
				this->onMouseDown->Event(this, GUIMouseRight,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_RBUTTONUP:
			if(this->onMouseUp)
				this->onMouseUp->Event(this, GUIMouseRight,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MBUTTONDBLCLK:
			if(this->onDblClick)
				this->onDblClick->Event(this, GUIMouseMiddle,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MBUTTONDOWN:
			if(this->onMouseDown)
				this->onMouseDown->Event(this, GUIMouseMiddle,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MBUTTONUP:
			if(this->onMouseUp)
				this->onMouseUp->Event(this, GUIMouseMiddle,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MOUSEHOVER:
			if(this->onMouseHover)
				this->onMouseHover->Event(this);
			break;
		case WM_MOUSELEAVE:
			if(this->onMouseLeave)
				this->onMouseLeave->Event(this);
			break;

		case WM_KEYDOWN:
			if(this->onKeyDown)
				this->onKeyDown->Event(this, wParam,
				(GUIShift)((GetKeyState(VK_CONTROL)<0 ? GUIShiftControl : 0) | (GetKeyState(VK_SHIFT)<0 ? GUIShiftShift : 0)));
			break;
		case WM_CHAR:
			if(this->onKeyPress)
				this->onKeyPress->Event(this, wParam,
				(GUIShift)((GetKeyState(VK_CONTROL)<0 ? GUIShiftControl : 0) | (GetKeyState(VK_SHIFT)<0 ? GUIShiftShift : 0)));
			break;
		case WM_KEYUP:
			if(this->onKeyUp)
				this->onKeyUp->Event(this, wParam,
				(GUIShift)((GetKeyState(VK_CONTROL)<0 ? GUIShiftControl : 0) | (GetKeyState(VK_SHIFT)<0 ? GUIShiftShift : 0)));
			break;

		case WM_PAINT:{
				RECT rc;
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
				GetClientRect(hWnd, &rc);
				HDC bDC;
				HBITMAP bOld;
				HFONT font;
				bDC = CreateCompatibleDC(hDC);
				bOld = (HBITMAP)SelectObject(bDC, CreateCompatibleBitmap(hDC, rc.right, rc.bottom));
				font = (HFONT)SelectObject(bDC, this->hFont);
					BitBlt(bDC, 0, 0, rc.right, rc.bottom, bDC, 0, 0, WHITENESS);
					if(this->onPaint) this->onPaint->Event(this, bDC);
					BitBlt(hDC, 0, 0, rc.right, rc.bottom, bDC, 0, 0, SRCCOPY);
				SelectObject(bDC, font);
				DeleteObject(SelectObject(bDC, bOld));
				DeleteDC(bDC);
			EndPaint(hWnd, &ps);
			return 0;}

		case WM_MOVE:
			if(this->onMove)
				this->onMove->Event(this, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MOVING:
			if(this->onMove)
				this->onMove->Event(this, ((RECT*)lParam)->left, ((RECT*)lParam)->top);
			break;

		case WM_SIZE:
			if(this->onResize)
				this->onResize->Event(this, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_SIZING:
			if(this->onResize)
				this->onResize->Event(this, ((RECT*)lParam)->right-((RECT*)lParam)->left, ((RECT*)lParam)->bottom-((RECT*)lParam)->top);
			break;
	}
	return GUIObject::handleMessage(hWnd, Msg, wParam, lParam);
}
