/*
	GUICore - GUIWindow
	 The win32 GUI framework library.
	 author: WolfgangKurz

	Base class for windows
	A standard unit of parent windows
*/

#include <malloc.h>
#include <memory.h>

#include "GUIWindow.hpp"
#include "GUIValues.h"

typedef struct WidgetControl {
	GUIWidget *widget;
	wchar_t *name;
} WidgetControl;

typedef struct WidgetList {
	int capacity;
	WidgetControl *controls;
} WidgetList;

GUIWindow::GUIWindow(UINT flag, UINT style, UINT exstyle, HWND parent) : GUIObject(flag, style, exstyle, parent){
	this->widgets = new WidgetList;
	memset(this->widgets, 0, sizeof(WidgetList));

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

	this->onMenuClick   = (GUIMenuEventP)NULL;

	SetWindowText(this->hWnd, L"GUIWindow");
}
GUIWindow::GUIWindow(int x, int y, int width, int height, UINT flag, UINT style, UINT exstyle, HWND parent) : GUIObject(x, y, width, height, flag, style, exstyle, parent){
	this->widgets = new WidgetList;
	memset(this->widgets, 0, sizeof(WidgetList));

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

	this->onMenuClick   = (GUIMenuEventP)NULL;

	SetWindowText(this->hWnd, L"GUIWindow");
}
GUIWindow::~GUIWindow(){
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

	if(this->onMenuClick) { delete this->onMenuClick;  this->onMenuClick=NULL; }

	if(this->widgets){
		WidgetList *widgets = (WidgetList*)this->widgets;
		int i;

		if(widgets->controls){
			for(i=0; i<widgets->capacity; i++){
				delete[] widgets->controls[i].name;
				delete widgets->controls[i].widget;
			}
			free(widgets->controls);
		}
		delete this->widgets;
	}
}

GUIWindow* GUIWindow::setParent(GUIObject *parent){
	GUIObject::setParent(parent);
	return this;
}

GUIWindow* GUIWindow::setFont(const wchar_t *fontName, int fontSize, int weight){
	GUIObject::setFont(fontName, fontSize, weight);
	return this;
}
GUIWindow* GUIWindow::setFont(HFONT font){
	GUIObject::setFont(font);
	return this;
}

GUIWindow* GUIWindow::invalidate(BOOL immediately){
	GUIObject::invalidate(immediately);
	return this;
}

GUIWindow* GUIWindow::show(int show){
	GUIObject::show(show);
	return this;
}
GUIWindow* GUIWindow::hide(){
	GUIObject::hide();
	return this;
}

GUIWindow* GUIWindow::move_center(){
	GUIObject::move_center();
	return this;
}
GUIWindow* GUIWindow::move(int x, int y){
	GUIObject::move(x, y);
	return this;
}
GUIWindow* GUIWindow::resize(int width, int height){
	GUIObject::resize(width, height);
	return this;
}

GUIWindow* GUIWindow::setText(const wchar_t *text){
	SetWindowText(this->hWnd, text);
	return this;
}
const wchar_t* GUIWindow::getText(){
	int len = GetWindowTextLength(this->hWnd);
	wchar_t *output = new wchar_t[len+1];

	memset(output, 0, sizeof(wchar_t)*(len+1));
	GetWindowText(this->hWnd, output, len+1);

	return (const wchar_t*)output;
}

GUIWidget* GUIWindow::addControl(wchar_t *name, GUIWidget *widget){
	WidgetList *widgets = (WidgetList*)this->widgets;
	WidgetControl control;
	GUIWidget *output = this->getControl(name);
	int len;

	if(output!=(GUIWidget*)NULL) return output;

	len = wcslen(name);

	widgets->capacity++;
	widgets->controls = (WidgetControl*)realloc(widgets->controls, sizeof(WidgetControl)*(widgets->capacity));

	control.widget = widget;
	control.name = new wchar_t[len+1];
	memset(control.name, 0, sizeof(wchar_t)*(len+1));
	wcscpy_s(control.name, len+1, name);
	widgets->controls[widgets->capacity-1] = control;

	widget->setFont(this->getFont());

	widget->setParent(this)
		->show()
		->invalidate();
	return widget;
}
GUIWidget* GUIWindow::getControl(wchar_t *name){
	WidgetList *widgets = (WidgetList*)this->widgets;
	int i;
	for(i=0; i<widgets->capacity; i++)
		if( _wcsicmp(widgets->controls[i].name, name)==0 )
			return widgets->controls[i].widget;
	return (GUIWidget*)NULL;
}
const wchar_t* GUIWindow::getControl(GUIObject *object){
	WidgetList *widgets = (WidgetList*)this->widgets;
	int i;
	for(i=0; i<widgets->capacity; i++)
		if( widgets->controls[i].widget==object )
			return widgets->controls[i].name;
	return (const wchar_t*)NULL;
}
int GUIWindow::removeControl(wchar_t *name){
	WidgetList *widgets = (WidgetList*)this->widgets;
	int i, j;

	for(i=0; i<widgets->capacity; i++){
		if( _wcsicmp(widgets->controls[i].name, name)==0 ){
			delete[] widgets->controls[i].name;
			delete widgets->controls[i].widget;

			for(j=i; j<widgets->capacity-1; j++)
				widgets->controls[j] = widgets->controls[j+1];

			widgets->capacity--;
			widgets->controls = (WidgetControl*)realloc(widgets->controls, sizeof(WidgetControl)*(widgets->capacity));
			return 1;
		}
	}
	return 0;
}

LRESULT GUIWindow::handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
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

		case WM_SETFOCUS:
			if(this->onFocus)
				this->onFocus->Event(this, (HWND)wParam);
			break;
		case WM_KILLFOCUS:
			if(this->onKillFocus)
				this->onKillFocus->Event(this, (HWND)wParam);
			break;

		case WM_COMMAND:
			if(lParam==(LPARAM)NULL)
				if(this->onMenuClick)
					this->onMenuClick->Event(this, (const wchar_t*)wParam);
			break;
	}
	return GUIObject::handleMessage(hWnd, Msg, wParam, lParam);
}
