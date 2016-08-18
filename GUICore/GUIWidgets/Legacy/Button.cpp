/*
	GUIWidgets - Legacy - Button
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Button class
*/

#include "../GUIWidgetOpt.hpp"

#include "Button.hpp"

using namespace GUILegacy;

LRESULT WINAPI GUIWidget_ButtonProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

Button::Button() : GUIWidget(0, 0, 0, 0){
	this->backColor = 0x000000;
	this->textColor = 0xFFFFFF;

	this->btnHWND = CreateWindow(L"BUTTON", L"", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, NULL, GetModuleHandle(NULL), NULL);

	btnProcPrev = (WNDPROC)SetWindowLongPtr(this->btnHWND, GWLP_WNDPROC, (LONG_PTR)((void*)GUIWidget_ButtonProc));
	SetWindowLongPtr(this->btnHWND, GWLP_USERDATA, (LONG_PTR)((void*)this));
	SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)((void*)this));

	wchar_t *def = L"Button";
	SetWindowText(this->btnHWND, def);
}
Button::Button(int x, int y, int width, int height, int style) : GUIWidget(x, y, width, height){
	this->backColor = 0x000000;
	this->textColor = 0xFFFFFF;

	this->btnHWND = CreateWindow(L"BUTTON", L"", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_VISIBLE | style, 0, 0, 0, 0, hWnd, NULL, GetModuleHandle(NULL), NULL);
	SetWindowPos(this->btnHWND, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
	
	btnProcPrev = (WNDPROC)SetWindowLongPtr(this->btnHWND, GWLP_WNDPROC, (LONG_PTR)((void*)GUIWidget_ButtonProc));
	SetWindowLongPtr(this->btnHWND, GWLP_USERDATA, (LONG_PTR)((void*)this));
	SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)((void*)this));

	wchar_t *def = L"Button";
	SetWindowText(this->btnHWND, def);
}
Button::~Button(){
	if( btnProcPrev!=(WNDPROC)NULL ){
		SetWindowLongPtr(this->hWnd, GWLP_WNDPROC, (LONG_PTR)((void*)btnProcPrev));
		btnProcPrev = (WNDPROC)NULL;
	}
}

Button* Button::invalidate(BOOL immediately){
	InvalidateRect(this->btnHWND, NULL, FALSE);
	if(immediately) UpdateWindow(this->btnHWND);
	return this;
}

Button* Button::move_center(){
	GUIWidget::move_center();
	return this;
}
Button* Button::move(int x, int y){
	((GUIWidget*)this)->move(x, y);
	return this;
}
Button* Button::resize(int width, int height){
	((GUIWidget*)this)->resize(width, height);
	return this;
}

Button* Button::setFont(const wchar_t *fontName, int fontSize, int weight){
	GUIObject::setFont(fontName, fontSize, weight);
	SendMessage(this->btnHWND, WM_SETFONT, (WPARAM)this->getFont(), MAKELPARAM(TRUE, 0));
	return this;
}
Button* Button::setFont(HFONT font){
	GUIObject::setFont(font);
	SendMessage(this->btnHWND, WM_SETFONT, (WPARAM)this->getFont(), MAKELPARAM(TRUE, 0));
	return this;
}
HFONT Button::getFont(){
	return GUIObject::getFont();
}

Button* Button::show(){
	GUIWidget::show();
	return this;
}
Button* Button::hide(){
	GUIWidget::hide();
	return this;
}

WNDPROC Button::getButtonProc(){
	return this->btnProcPrev;
}

Button* Button::setBackColor(int color){
	this->backColor = color;
	InvalidateRect(this->btnHWND, NULL, TRUE);
	return this;
}
int Button::getBackColor(){
	return this->backColor;
}

Button* Button::setText(const wchar_t *text){
	SetWindowText(this->btnHWND, text);
	return this;
}
const wchar_t* Button::getText(){
	int len = GetWindowTextLength(this->btnHWND);
	wchar_t *out = new wchar_t[len+1];
	GetWindowText(this->btnHWND, out, len+1);
	return out;
}

LRESULT Button::handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	switch(Msg){
		case WM_SIZE: case WM_SIZING:{
			RECT rc;
			GetClientRect(hWnd, &rc);
			SetWindowPos(this->btnHWND, HWND_TOP, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER | SWP_NOACTIVATE);
			break;}
		case WM_CTLCOLORBTN:{
			HDC hDC = (HDC)wParam;
			SetBkColor(hDC, this->backColor);
			SetDCBrushColor(hDC, this->backColor);
			SetTextColor(hDC, this->textColor);
			return (LRESULT)GetStockObject(DC_BRUSH);}
	}
	return GUIWidget::handleMessage(hWnd, Msg, wParam, lParam);
}

LRESULT WINAPI GUIWidget_ButtonProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	Button *textBox = (Button*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	
	switch(Msg){
		case WM_MOUSEMOVE:
			if(textBox->onMouseMove)
				textBox->onMouseMove->Event(textBox,
					(GUIMouse)(((wParam&MK_LBUTTON)==MK_LBUTTON ? GUIMouseLeft : 0) | ((wParam&MK_RBUTTON)==MK_RBUTTON ? GUIMouseRight : 0) | ((wParam&MK_MBUTTON)==MK_MBUTTON ? GUIMouseMiddle : 0)),
					(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
					LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONDBLCLK:
			if(textBox->onDblClick)
				textBox->onDblClick->Event(textBox, GUIMouseLeft,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONDOWN:
			if(textBox->onMouseDown)
				textBox->onMouseDown->Event(textBox, GUIMouseLeft,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONUP:
			if(textBox->onMouseUp){
				if( textBox->onMouseUp->Event(textBox, GUIMouseLeft,
					(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
					LOWORD(lParam), HIWORD(lParam))!=0 )
					break;
			}
			if(textBox->onClick) textBox->onClick->Event(textBox, GUIMouseLeft,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_RBUTTONDBLCLK:
			if(textBox->onDblClick)
				textBox->onDblClick->Event(textBox, GUIMouseRight,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_RBUTTONDOWN:
			if(textBox->onMouseDown)
				textBox->onMouseDown->Event(textBox, GUIMouseRight,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_RBUTTONUP:
			if(textBox->onMouseUp)
				textBox->onMouseUp->Event(textBox, GUIMouseRight,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MBUTTONDBLCLK:
			if(textBox->onDblClick)
				textBox->onDblClick->Event(textBox, GUIMouseMiddle,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MBUTTONDOWN:
			if(textBox->onMouseDown)
				textBox->onMouseDown->Event(textBox, GUIMouseMiddle,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MBUTTONUP:
			if(textBox->onMouseUp)
				textBox->onMouseUp->Event(textBox, GUIMouseMiddle,
				(GUIShift)(((wParam&MK_CONTROL)==MK_CONTROL ? GUIShiftControl : 0) | ((wParam&MK_SHIFT)==MK_SHIFT ? GUIShiftShift : 0)),
				LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_KEYDOWN:
			if(textBox->onKeyDown)
				textBox->onKeyDown->Event(textBox, wParam,
				(GUIShift)((GetKeyState(VK_CONTROL)<0 ? GUIShiftControl : 0) | (GetKeyState(VK_SHIFT)<0 ? GUIShiftShift : 0)));
			break;
		case WM_CHAR:
			if(textBox->onKeyPress)
				textBox->onKeyPress->Event(textBox, wParam,
				(GUIShift)((GetKeyState(VK_CONTROL)<0 ? GUIShiftControl : 0) | (GetKeyState(VK_SHIFT)<0 ? GUIShiftShift : 0)));
			break;
		case WM_KEYUP:
			if(textBox->onKeyUp)
				textBox->onKeyUp->Event(textBox, wParam,
				(GUIShift)((GetKeyState(VK_CONTROL)<0 ? GUIShiftControl : 0) | (GetKeyState(VK_SHIFT)<0 ? GUIShiftShift : 0)));
			break;
	}

	return CallWindowProc(textBox->getButtonProc(), hWnd, Msg, wParam, lParam);
}
