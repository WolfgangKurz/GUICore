/*
	GUIWidgets - Legacy - TextBox
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	TextBox class
*/

#include "../GUIWidgetOpt.hpp"

#include "TextBox.hpp"

using namespace GUILegacy;

LRESULT WINAPI GUIWidget_TextBoxProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

TextBox::TextBox() : GUIWidget(0, 0, 0, 0){
	this->backColor = 0x000000;
	this->textColor = 0xFFFFFF;

	this->editHWND = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, NULL, GetModuleHandle(NULL), NULL);

	editProcPrev = (WNDPROC)SetWindowLongPtr(this->editHWND, GWLP_WNDPROC, (LONG_PTR)((void*)GUIWidget_TextBoxProc));
	SetWindowLongPtr(this->editHWND, GWLP_USERDATA, (LONG_PTR)((void*)this));
	SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)((void*)this));

	wchar_t *def = L"TextBox";
	SetWindowText(this->editHWND, def);

	onChange = (GUIEventP)NULL;
}
TextBox::TextBox(int x, int y, int width, int height, int style) : GUIWidget(x, y, width, height){
	this->backColor = 0x000000;
	this->textColor = 0xFFFFFF;

	this->editHWND = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | style, 0, 0, 0, 0, hWnd, NULL, GetModuleHandle(NULL), NULL);
	SetWindowPos(this->editHWND, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
	
	editProcPrev = (WNDPROC)SetWindowLongPtr(this->editHWND, GWLP_WNDPROC, (LONG_PTR)((void*)GUIWidget_TextBoxProc));
	SetWindowLongPtr(this->editHWND, GWLP_USERDATA, (LONG_PTR)((void*)this));
	SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)((void*)this));

	wchar_t *def = L"TextBox";
	SetWindowText(this->editHWND, def);

	onChange = (GUIEventP)NULL;
}
TextBox::~TextBox(){
	if( editProcPrev!=(WNDPROC)NULL ){
		SetWindowLongPtr(this->hWnd, GWLP_WNDPROC, (LONG_PTR)((void*)editProcPrev));
		editProcPrev = (WNDPROC)NULL;
	}
}

TextBox* TextBox::invalidate(BOOL immediately){
	InvalidateRect(this->editHWND, NULL, FALSE);
	if(immediately) UpdateWindow(this->editHWND);
	return this;
}

TextBox* TextBox::move_center(){
	GUIWidget::move_center();
	return this;
}
TextBox* TextBox::move(int x, int y){
	((GUIWidget*)this)->move(x, y);
	return this;
}
TextBox* TextBox::resize(int width, int height){
	((GUIWidget*)this)->resize(width, height);
	return this;
}

TextBox* TextBox::setFont(const wchar_t *fontName, int fontSize, int weight){
	GUIObject::setFont(fontName, fontSize, weight);
	SendMessage(this->editHWND, WM_SETFONT, (WPARAM)this->getFont(), MAKELPARAM(TRUE, 0));
	return this;
}
TextBox* TextBox::setFont(HFONT font){
	GUIObject::setFont(font);
	SendMessage(this->editHWND, WM_SETFONT, (WPARAM)this->getFont(), MAKELPARAM(TRUE, 0));
	return this;
}
HFONT TextBox::getFont(){
	return GUIObject::getFont();
}

TextBox* TextBox::show(){
	GUIWidget::show();
	return this;
}
TextBox* TextBox::hide(){
	GUIWidget::hide();
	return this;
}

WNDPROC TextBox::getEditProc(){
	return this->editProcPrev;
}

TextBox* TextBox::setBackColor(int color){
	this->backColor = color;
	InvalidateRect(this->editHWND, NULL, TRUE);
	return this;
}
int TextBox::getBackColor(){
	return this->backColor;
}

TextBox* TextBox::setTextColor(int color){
	this->textColor = color;
	InvalidateRect(this->editHWND, NULL, TRUE);
	return this;
}
int TextBox::getTextColor(){
	return this->textColor;
}

TextBox* TextBox::setText(const wchar_t *text){
	SetWindowText(this->editHWND, text);
	return this;
}
const wchar_t* TextBox::getText(){
	int len = GetWindowTextLength(this->editHWND);
	wchar_t *out = new wchar_t[len+1];
	GetWindowText(this->editHWND, out, len+1);
	return out;
}

LRESULT TextBox::handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	switch(Msg){
		case WM_SIZE: case WM_SIZING:{
			RECT rc;
			GetClientRect(hWnd, &rc);
			SetWindowPos(this->editHWND, HWND_TOP, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER | SWP_NOACTIVATE);
			break;}
		case WM_NOTIFY:{
			NMHDR *hdr = (NMHDR*)lParam;
			switch( hdr->code ){
				case EN_CHANGE:
					if(this->onChange) this->onChange->Event(this);
			}
			break;}
		case WM_CTLCOLOREDIT:{
			HDC hDC = (HDC)wParam;
			SetBkColor(hDC, this->backColor);
			SetDCBrushColor(hDC, this->backColor);
			SetTextColor(hDC, this->textColor);
			return (LRESULT)GetStockObject(DC_BRUSH);}
	}
	return GUIWidget::handleMessage(hWnd, Msg, wParam, lParam);
}

LRESULT WINAPI GUIWidget_TextBoxProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	TextBox *textBox = (TextBox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	
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

	return CallWindowProc(textBox->getEditProc(), hWnd, Msg, wParam, lParam);
}
