/*
	GUIWidgets - Neo - Statusbar
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Statusbar class
*/

#include "../GUIWidgetOpt.hpp"

#include "Statusbar.hpp"

using namespace GUINeo;

Statusbar::Statusbar(int height) : GUIWidget(0, 0, 0, height){
	this->resizable = FALSE;
	this->backColor = 0xFFFFFF;
	this->textColor = 0x000000;

	wchar_t *def = L"Statusbar";
	int len = wcslen(def);
	this->text = new wchar_t[len+1];
	wcscpy_s(this->text, len+1, def);
}
Statusbar::~Statusbar(){
	if(this->text) delete[] this->text;
}

Statusbar* Statusbar::invalidate(BOOL immediately){
	GUIWidget::invalidate(immediately);
	return this;
}

Statusbar* Statusbar::move_center(){
	return this->locate();
}
Statusbar* Statusbar::move(int, int){
	return this->locate();
}
Statusbar* Statusbar::resize(int, int){
	return this->locate();
}
Statusbar* Statusbar::locate(){
	int pr, pb;
	pr = this->parent->getInnerWidth();
	pb = this->parent->getInnerHeight();
	SetWindowPos(hWnd, HWND_TOP, 0, pb-this->getHeight(), pr, this->getHeight(), SWP_NOZORDER);
	return this;
}

Statusbar* Statusbar::show(){
	GUIWidget::show();
	return this;
}
Statusbar* Statusbar::hide(){
	GUIWidget::hide();
	return this;
}

Statusbar* Statusbar::setResizable(BOOL resizable){
	this->resizable = (resizable==FALSE ? FALSE : TRUE);
	this->invalidate();
	return this;
}
BOOL Statusbar::getResizable(){
	return this->resizable;
}

Statusbar* Statusbar::setBackColor(int color){
	this->backColor = color;
	this->invalidate();
	return this;
}
int Statusbar::getBackColor(){
	return this->backColor;
}

Statusbar* Statusbar::setTextColor(int color){
	this->textColor = color;
	this->invalidate();
	return this;
}
int Statusbar::getTextColor(){
	return this->textColor;
}

Statusbar* Statusbar::setText(const wchar_t *text){
	int len = wcslen(text);

	if(this->text) delete[] this->text;

	this->text = new wchar_t[len+1];
	wcscpy_s(this->text, len+1, text);

	this->invalidate();
	return this;
}
const wchar_t* Statusbar::getText(){
	return (const wchar_t*)this->text;
}

LRESULT Statusbar::handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	switch(Msg){
		case WM_MOVE:
		case WM_SIZE:{
			RECT rc;
			GetClientRect(GetParent(hWnd), &rc);
			SetWindowPos(hWnd, HWND_TOP, 0, rc.bottom-this->getHeight(), rc.right, this->getHeight(), SWP_NOZORDER);
			break;}
		case WM_MOVING:
		case WM_SIZING:{
			RECT rc, *rct = (RECT*)lParam;
			GetClientRect(GetParent(hWnd), &rc);
			rct->left = 0;
			rct->top = rc.bottom-this->getHeight();
			rct->right = rc.right;
			rct->bottom = rc.bottom;
			break;}

		case WM_MOUSEMOVE:
			if(this->resizable==TRUE){
				POINTS pt = MAKEPOINTS(lParam);
				int w = this->getWidth(), h = this->getHeight();

				if(pt.x>=w-12 && pt.y>=1){ // Right Bottom
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_SIZENWSE));
				}else if(pt.x<4 && pt.y>=h-4){ // Left Bottom
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_SIZENESW));
				}else if(pt.x<4){ // Left
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_SIZEWE));
				}else if(pt.x>=w-4){ // Right
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_SIZEWE));
				}else if(pt.y>=h-4){ // Bottom
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_SIZENS));
				}
			}
			break;
		case WM_LBUTTONDOWN:
			if(this->resizable==TRUE){
				POINTS pt = MAKEPOINTS(lParam);
				POINT pts = {pt.x, pt.y};
				int w = this->getWidth(), h = this->getHeight();
				ClientToScreen(this->hWnd, &pts);

				if(pt.x>=w-12 && pt.y>=1){ // Right Bottom
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_SIZENWSE));
					SendMessage(this->parent->getHWND(), WM_NCLBUTTONDOWN, HTBOTTOMRIGHT, MAKELPARAM(pts.x, pts.y));
				}else if(pt.x<4 && pt.y>=h-4){ // Left Bottom
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_SIZENESW));
					SendMessage(this->parent->getHWND(), WM_NCLBUTTONDOWN, HTBOTTOMLEFT, MAKELPARAM(pts.x, pts.y));
				}else if(pt.x<4){ // Left
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_SIZEWE));
					SendMessage(this->parent->getHWND(), WM_NCLBUTTONDOWN, HTLEFT, MAKELPARAM(pts.x, pts.y));
				}else if(pt.x>=w-4){ // Right
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_SIZEWE));
					SendMessage(this->parent->getHWND(), WM_NCLBUTTONDOWN, HTRIGHT, MAKELPARAM(pts.x, pts.y));
				}else if(pt.y>=h-4){ // Bottom
					SetCursor(LoadCursor((HINSTANCE)NULL, IDC_SIZENS));
					SendMessage(this->parent->getHWND(), WM_NCLBUTTONDOWN, HTBOTTOM, MAKELPARAM(pts.x, pts.y));
				}
			}
			break;

		case WM_PAINT:{
			RECT rc;
			GetClientRect(hWnd, &rc);

			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
				SetTextColor(hDC, this->textColor);
				SetBkColor(hDC, this->backColor);

				HFONT font = (HFONT)SelectObject(hDC, this->getFont());
				HBRUSH hbr = CreateSolidBrush(this->backColor);
					SIZE sz;
					GetTextExtentPoint32(hDC, this->text, wcslen(this->text), &sz);

					FillRect(hDC, &rc, hbr);
					TextOut(hDC, 6, this->getHeight()/2-sz.cy/2, this->text, wcslen(this->text));

					if(this->resizable==TRUE){
						int w = this->getWidth(), h = this->getHeight();
						int b = this->backColor;
						b = RGB((b&0xff)/3*2, ((b>>8)&0xff)/3*2, ((b>>16)&0xff)/3*2);

						for(int i=0; i<4; i++){
							for(int j=i; j<4; j++){
								SetPixelV(hDC, w-10+j*2, h-4-i*2, this->textColor);
								SetPixelV(hDC, w-11+j*2, h-5-i*2, b);
							}
						}
					}
				DeleteObject(hbr);
				SelectObject(hDC, font);
			EndPaint(hWnd, &ps);
			return 0;}
	}
	return GUIWidget::handleMessage(hWnd, Msg, wParam, lParam);
}
