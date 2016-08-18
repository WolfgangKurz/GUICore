/*
	GUIWidgets - WidgetMenu
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Menu widget class, require Menu
*/

#include "../GUIWidgetOpt.hpp"

#include "WidgetMenu.hpp"
#include "../Legacy/MenuItem.hpp"

using namespace GUINeo;
using namespace GUILegacy; // Uses Legacy MenuItem

WidgetMenu::WidgetMenu(int x, int y, int width, int height) : GUIWidget(x, y, width, height){
	this->backColor = 0x000000;
	this->activeBackColor = 0x444444;
	this->textColor = 0xFFFFFF;

	this->index = -1;
	this->state = 0;

	this->root = new MenuItem(L"", L"", (MenuItem*)NULL);
	this->root->setRoot(this);

	this->onMenuClick = (GUIMenuEventP)NULL;
}
WidgetMenu::~WidgetMenu(){
	if(this->root) delete this->root;
	if(this->onMenuClick) delete this->onMenuClick;
}

WidgetMenu* WidgetMenu::setFont(const wchar_t *fontName, int fontSize, int weight){
	GUIWidget::setFont(fontName, fontSize, weight);
	this->invalidate();
	return this;
}
WidgetMenu* WidgetMenu::setFont(HFONT font){
	GUIWidget::setFont(font);
	this->invalidate();
	return this;
}

WidgetMenu* WidgetMenu::invalidate(BOOL immediately){
	GUIWidget::invalidate(immediately);
	return this;
}

WidgetMenu* WidgetMenu::move_center(){
	GUIWidget::move_center();
	return this;
}
WidgetMenu* WidgetMenu::move(int x, int y){
	GUIWidget::move(x, y);
	return this;
}
WidgetMenu* WidgetMenu::resize(int width, int height){
	GUIWidget::resize(width, height);
	return this;
}
WidgetMenu* WidgetMenu::locate(){
	RECT rc;
	GetClientRect(GetParent(hWnd), &rc);
	SetWindowPos(hWnd, HWND_TOP, 0, 0, rc.right, this->getHeight(), SWP_NOZORDER);
	return this;
}

WidgetMenu* WidgetMenu::show(){
	GUIWidget::show();
	return this;
}
WidgetMenu* WidgetMenu::hide(){
	GUIWidget::hide();
	return this;
}

WidgetMenu* WidgetMenu::setBackColor(int color){
	this->backColor = color;
	this->invalidate();
	return this;
}
int WidgetMenu::getBackColor(){
	return this->backColor;
}

WidgetMenu* WidgetMenu::setActiveBackColor(int color){
	this->activeBackColor = color;
	this->invalidate();
	return this;
}
int WidgetMenu::getActiveBackColor(){
	return this->activeBackColor;
}

WidgetMenu* WidgetMenu::setHoverColor(int color){
	this->hoverColor = color;
	this->invalidate();
	return this;
}
int WidgetMenu::getHoverColor(){
	return this->hoverColor;
}

WidgetMenu* WidgetMenu::setTextColor(int color){
	this->textColor = color;
	this->invalidate();
	return this;
}
int WidgetMenu::getTextColor(){
	return this->textColor;
}

int WidgetMenu::getChildCount(){
	return this->root->getChildCount();
}
MenuItem* WidgetMenu::getChild(int index){
	return this->root->getChild(index);
}

MenuItem* WidgetMenu::addChild(const wchar_t *key, const wchar_t *text){
	MenuItem *item = this->root->addChild(key, text);
	this->invalidate();
	return item;
}
MenuItem* WidgetMenu::addChild(const wchar_t *key, const wchar_t *text, int index){
	MenuItem *item = this->root->addChild(key, text, index);
	this->invalidate();
	return item;
}
WidgetMenu* WidgetMenu::removeChild(int index){
	this->root->removeChild(index);
	return this;
}

WidgetMenu* WidgetMenu::popupShow(int x, int y){
	HWND parentHWND = (HWND)NULL;
	if(this->root){
		GUIObject *parent = this->getParent();
		if(parent) parentHWND = parent->getHWND();
	}
	
	HMENU menu = this->root->buildMenu(TRUE);
		UINT ret = TrackPopupMenu(menu, TPM_WORKAREA | TPM_RIGHTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, x, y, 0, parentHWND, (RECT*)NULL);

		if(this->onMenuClick)
			this->onMenuClick->Event(this, (const wchar_t*)ret);
	DestroyMenu(menu);
	return this;
}

LRESULT WidgetMenu::handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	switch(Msg){
		case WM_PAINT:{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
				HBRUSH hbr;
				RECT rc;
				GetClientRect(hWnd, &rc);

				hbr = CreateSolidBrush(this->backColor);
					SetBkMode(hDC, TRANSPARENT);
					FillRect(hDC, &rc, hbr);
				DeleteObject(hbr);

				HFONT font = (HFONT)SelectObject(hDC, this->getFont());
					int w=0;
					for(int i=0; i<this->getChildCount(); i++){
						MenuItem *child = this->getChild(i);
						const wchar_t *text = child->getText();
						SIZE sz;

						if(child->getEnabled()==FALSE)
							SetTextColor(hDC, this->activeBackColor);
						else
							SetTextColor(hDC, this->textColor);

						GetTextExtentPoint32(hDC, text, wcslen(text), &sz);
						SetRect(&rc, w, 0, w+sz.cx+20, this->getHeight());

						int bc = this->backColor;
						if(index==i){
							if(state>0){
								bc = this->activeBackColor;
							}else{
								bc = this->hoverColor;
							}
						}

						hbr = CreateSolidBrush(bc);
							FillRect(hDC, &rc, hbr);
						DeleteObject(hbr);

						DrawText(hDC, text, wcslen(text), &rc, DT_CENTER | DT_VCENTER| DT_SINGLELINE);

						w += sz.cx+20;
					}
				SelectObject(hDC, font);
			EndPaint(hWnd, &ps);
			return 0;}
		case WM_LBUTTONDOWN:{
			int prev = this->state;
			this->state = 0;

			if(this->index>=0){
				this->state = 1;
				this->invalidate(TRUE);

				MenuItem *child = this->getChild(this->index);
				
				if(child->getChildCount()>0){
					int x = 0;
					HDC hDC = GetDC(hWnd);
					HFONT font = (HFONT)SelectObject(hDC, this->getFont());
						for(int i=0; i<this->index; i++){
							const wchar_t *text = this->getChild(i)->getText();
							SIZE sz;

							GetTextExtentPoint32(hDC, text, wcslen(text), &sz);
							x += sz.cx+20;
						}
					SelectObject(hDC, font);
					ReleaseDC(hWnd, hDC);

					this->invalidate(TRUE);

					POINT pt = {x, this->getHeight()};
					ClientToScreen(this->hWnd, &pt);

					HMENU menu = this->getChild(this->index)->buildMenu(TRUE);
						UINT ret = TrackPopupMenu(menu, TPM_WORKAREA | TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, pt.x, pt.y, 0, this->getHWND(), (RECT*)NULL);

						if(this->onMenuClick) this->onMenuClick->Event(this, (const wchar_t*)ret);
					DestroyMenu(menu);
				}
			}

			if(this->state!=prev) this->invalidate();
			break;}
		case WM_LBUTTONUP:{
			int prev = this->state;
			this->state = 0;

			ReleaseCapture();

			if(this->index>=0){
				MenuItem *child = this->getChild(this->index);
				
				if(child->getChildCount()==0){
					const wchar_t *key = child->getKey();

					if(child->onMenuClick) child->onMenuClick->Event(this, key);
					if(this->onMenuClick) this->onMenuClick->Event(this, key);

					this->index = -1;
					this->invalidate();
				}
			}
			if(this->state!=prev) this->invalidate();
			break;}
		case WM_MOUSELEAVE:
			if(this->state!=2){
				this->index = -1;
				this->state = 0;
				this->invalidate();
			}
			break;
		case WM_MOUSEMOVE:{
			POINTS pt = MAKEPOINTS(lParam);

			int w=0, x=0;
			HDC hDC = GetDC(hWnd);
			HFONT font = (HFONT)SelectObject(hDC, this->getFont());
				int prev = this->index;
				this->index = -1;

				if(pt.y>=0 && pt.y<this->getHeight()){
					for(int i=0; i<this->getChildCount(); i++){
						const wchar_t *text = this->getChild(i)->getText();
						SIZE sz;

						GetTextExtentPoint32(hDC, text, wcslen(text), &sz);
						if( pt.x>=w && pt.x<w+sz.cx+20 && this->getChild(i)->getEnabled()>0 ){
							this->index = i;
							x = w;
							break;
						}

						w += sz.cx+20;
					}
				}
			SelectObject(hDC, font);
			ReleaseDC(hWnd, hDC);

			if(prev!=this->index) this->invalidate();
			break;}
	}
	return GUIWidget::handleMessage(hWnd, Msg, wParam, lParam);
}
