/*
	GUIWidgets - Neo - Button
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Button class
*/

#include "../GUIWidgetOpt.hpp"

#include "Button.hpp"

using namespace GUINeo;

Button::Button() : GUIWidget(0, 0, 0, 0){
	this->backColor = 0x000000;
	this->activeBackColor = 0x444444;
	this->textColor = 0xFFFFFF;
	this->hoverColor = -1; // transparent
	this->enabled = 1;
	this->align = GUITextAlignMiddle; // center

	wchar_t *def = L"Button";
	int len = wcslen(def);
	this->text = new wchar_t[len+1];
	wcscpy_s(this->text, len+1, def);

	state = 0;
}
Button::Button(int x, int y, int width, int height) : GUIWidget(x, y, width, height){
	this->backColor = 0x000000;
	this->activeBackColor = 0x444444;
	this->textColor = 0xFFFFFF;
	this->hoverColor = -1; // transparent
	this->enabled = 1;
	this->align = GUITextAlignMiddle; // center

	wchar_t *def = L"Button";
	int len = wcslen(def);
	this->text = new wchar_t[len+1];
	wcscpy_s(this->text, len+1, def);

	state = 0;
}
Button::~Button(){
	if(this->text!=(wchar_t*)NULL) delete[] this->text;
}

Button* Button::invalidate(BOOL immediately){
	GUIWidget::invalidate(immediately);
	return this;
}

Button* Button::move_center(){
	GUIWidget::move_center();
	return this;
}
Button* Button::move(int x, int y){
	GUIWidget::move(x, y);
	return this;
}
Button* Button::resize(int width, int height){
	GUIWidget::resize(width, height);
	return this;
}

Button* Button::show(){
	GUIWidget::show();
	return this;
}
Button* Button::hide(){
	GUIWidget::hide();
	return this;
}

Button* Button::setEnabled(int enabled){
	this->enabled = enabled;
	this->invalidate();
	return this;
}
int Button::getEnabled(){
	return this->enabled;
}

Button* Button::setBackColor(int color){
	this->backColor = color;
	this->invalidate();
	return this;
}
int Button::getBackColor(){
	return this->backColor;
}

Button* Button::setActiveBackColor(int color){
	this->activeBackColor = color;
	this->invalidate();
	return this;
}
int Button::getActiveBackColor(){
	return this->activeBackColor;
}

Button* Button::setHoverColor(int color){
	this->hoverColor = color;
	this->invalidate();
	return this;
}
int Button::getHoverColor(){
	return this->hoverColor;
}

Button* Button::setTextColor(int color){
	this->textColor = color;
	this->invalidate();
	return this;
}
int Button::getTextColor(){
	return this->textColor;
}

Button* Button::setText(const wchar_t *text){
	int len = wcslen(text);

	if(this->text) delete[] this->text;

	this->text = new wchar_t[len+1];
	wcscpy_s(this->text, len+1, text);

	this->invalidate();
	return this;
}
const wchar_t* Button::getText(){
	return (const wchar_t*)this->text;
}

Button* Button::setAlign(GUITextAlign align){
	this->align = align;
	this->invalidate();
	return this;
}
GUITextAlign Button::getAlign(){
	return this->align;
}

LRESULT Button::handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	switch(Msg){
		case WM_MOUSEMOVE:{
			if(state<2){
				state = 1;
				this->invalidate();
			}
			break;}
		case WM_MOUSELEAVE:
			if(state!=2){
				state = 0;
				this->invalidate();
			}
			break;
		case WM_LBUTTONDOWN:
			if(GetCapture()!=this->hWnd) SetCapture(this->hWnd);

			state = 2;
			this->invalidate();
			break;
		case WM_LBUTTONUP:{
			ReleaseCapture();

			int px = LOWORD(lParam), py = HIWORD(lParam);
			POINT pt = {px, py};
			ClientToScreen(this->hWnd, &pt);

			if(WindowFromPoint(pt)!=this->hWnd){
				state = 0;
				this->invalidate();
				return 0;
			}else{
				state = 1;
			}
			this->invalidate();
			break;}
		case WM_PAINT:{
			RECT rc;
			GetClientRect(hWnd, &rc);

			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps); {
				int bcolor;
				if(this->state==0) bcolor = this->backColor;
				else if(this->state==1) bcolor = this->hoverColor==-1 ? this->backColor : this->hoverColor;
				else bcolor = this->activeBackColor;

				SetTextColor(hDC, this->textColor);
				SetBkColor(hDC, bcolor);

				HFONT font = (HFONT)SelectObject(hDC, this->getFont());
				HBRUSH hbr = CreateSolidBrush(GetBkColor(hDC));
				SIZE sz;

				GetTextExtentPoint32(hDC, this->text, wcslen(this->text), &sz);

				FillRect(hDC, &rc, hbr);
				SetRect(&rc, 0, this->getHeight()/2-sz.cy/2, rc.right, 0);
				rc.left += rc.top;
				rc.right -= rc.top;
				rc.bottom = rc.top+sz.cy;

				int align = 0;
				if(this->align==GUITextAlignLeft) align = DT_LEFT;
				else if(this->align==GUITextAlignRight) align = DT_RIGHT;
				else if(this->align==GUITextAlignMiddle) align = DT_CENTER;

				DrawText(hDC, this->text, wcslen(this->text), &rc, align);

				DeleteObject(hbr);
				SelectObject(hDC, font);
			}
			EndPaint(hWnd, &ps);
			return 0;}
	}
	if( !(Msg==WM_MOVE || Msg==WM_MOVING || Msg==WM_SIZE || Msg==WM_SIZING || Msg==WM_PAINT) ){
		if( this->enabled==0 ) return 0;
	}
	return GUIWidget::handleMessage(hWnd, Msg, wParam, lParam);
}
