/*
	GUIWidgets - Menu
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Menu class, require Button class
*/

#include "../GUIWidgetOpt.hpp"

#include "MenuItem.hpp"
#include "Menu.hpp"

using namespace GUILegacy;

class MenuItemClick : public GUIMenuEvent {
private:
	Menu *menu;

public:
	MenuItemClick(Menu *menu){
		this->menu = menu;
	}
	virtual long Event(GUIObject *sender, const wchar_t *key){
		if(this->menu){
			if(this->menu->onMenuClick)
				return this->menu->onMenuClick->Event(sender, key);
		}
		return 0;
	}
};

Menu::Menu() : GUIWidget(0, 0, 0, 0){
	this->root = new MenuItem(L"", L"", (MenuItem*)NULL);
	this->prevMenu = (HMENU)NULL;

	this->onMenuClick = (GUIMenuEventP)NULL;
}
Menu::Menu(MenuItem *menu) : GUIWidget(0, 0, 0, 0){
	this->root = menu;
	this->prevMenu = (HMENU)NULL;

	this->onMenuClick = (GUIMenuEventP)NULL;
}
Menu::~Menu(){
	this->hide();
	if(this->root){ delete this->root; this->root=NULL; }

	if(this->onMenuClick){ delete this->onMenuClick; this->onMenuClick=NULL; }
}

Menu* Menu::setParent(GUIObject *parent){
	GUIWidget::setParent(parent);
	this->show();
	return this;
}

Menu* Menu::show(){
	if(this->parent!=(GUIObject*)NULL){
		this->hide();

		GUIWindow *window = (GUIWindow*)this->parent;
		HMENU hMenu = this->root->buildMenu(FALSE);
			SetMenu(window->getHWND(), hMenu);

			window->onMenuClick = new MenuItemClick(this);
		this->prevMenu = hMenu;
	}
	return this;
}
Menu* Menu::hide(){
	if(this->parent!=(GUIObject*)NULL){
		GUIWindow *window = (GUIWindow*)this->parent;

		if(window->onMenuClick){ delete window->onMenuClick; window->onMenuClick=NULL; }
		window->onMenuClick = (GUIMenuEventP)NULL;

		SetMenu(this->parent->getHWND(), (HMENU)NULL);
	}
	if(this->prevMenu){
		DestroyMenu(this->prevMenu);
		this->prevMenu = (HMENU)NULL;
	}
	return this;
}

int Menu::getChildCount(){
	return this->root->getChildCount();
}
MenuItem* Menu::getChild(int index){
	return this->root->getChild(index);
}

MenuItem* Menu::addChild(const wchar_t *key, const wchar_t *text){
	return this->root->addChild(key, text);
}
MenuItem* Menu::addChild(const wchar_t *key, const wchar_t *text, int index){
	return this->root->addChild(key, text, index);
}
Menu* Menu::removeChild(int index){
	this->root->removeChild(index);
	return this;
}

Menu* Menu::popupShow(int x, int y){
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
