/*
	GUIWidgets - MenuItem
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Item of Menu class
	need to use Menu
*/

#include "../GUIWidgetOpt.hpp"

#include "MenuItem.hpp"
#include "Menu.hpp"

using namespace GUILegacy;

MenuItem::MenuItem(const wchar_t *key, const wchar_t *text, MenuItem *parent){
	this->onMenuClick = (GUIMenuEventP)NULL;

	this->parent = parent;
	this->root = (Menu*)NULL;

	this->childCount = 0;
	this->childs = (MenuItem**)NULL;
	this->enabled = TRUE;

	this->text = (wchar_t*)NULL;
	this->setText(text);

	int len = wcslen(key);
	this->key = new wchar_t[len+1];
	wcscpy_s(this->key, len+1, key);
}
MenuItem::~MenuItem(){
	if(this->childs!=(MenuItem**)NULL){
		for(int i=0; i<this->childCount; i++)
			delete this->childs[i];
		delete[] this->childs;
	}
	if(this->key!=(wchar_t*)NULL) delete[] this->key;
	if(this->text!=(wchar_t*)NULL) delete[] this->text;

	if(this->onMenuClick) delete this->onMenuClick;
}

MenuItem* MenuItem::clone(MenuItem *parent){
	MenuItem *output = new MenuItem(this->key, this->text, parent);

	for(int i=0; i<this->childCount; i++){
		MenuItem *child = this->childs[i];
		output->addChild( child->clone(output) );
	}
	return output;
}

MenuItem* MenuItem::setEnabled(BOOL enabled){
	this->enabled = enabled==0 ? 0 : 1;
	return this;
}
BOOL MenuItem::getEnabled(){
	return this->enabled;
}

MenuItem* MenuItem::setRoot(GUIWidget *root){
	this->root = root;
	return this;
}
GUIWidget* MenuItem::getRoot(){
	return this->root;
}
MenuItem* MenuItem::getParent(){
	return this->parent;
}

HMENU MenuItem::buildMenu(BOOL popup){
	HMENU base;

	if(popup)
		base = CreatePopupMenu();
	else
		base = CreateMenu();

	this->stackMenu(base);
	return base;
}
void MenuItem::stackMenu(HMENU baseMenu){
	for(int i=0; i<this->childCount; i++){
		MenuItem *item = this->childs[i];
		UINT flag = (item->getEnabled()==FALSE ? MF_DISABLED | MF_GRAYED : MF_ENABLED);

		if(item->getChildCount()>0){
			AppendMenu(baseMenu, MF_STRING | MF_POPUP | flag, (UINT_PTR)CreatePopupMenu(), item->getText());
			item->stackMenu(GetSubMenu(baseMenu, i));
		}else{
			const wchar_t *key = item->getKey();
			const wchar_t *text = item->getText();

			if(wcslen(text)==1 && text[0]==L'-')
				AppendMenu(baseMenu, MF_SEPARATOR, 0, (LPCWSTR)NULL);
			else
				AppendMenu(baseMenu, MF_STRING | flag, (UINT_PTR)key, text);
		}
	}
}

int MenuItem::getChildCount(){
	return this->childCount;
}
MenuItem* MenuItem::getChild(int index){
	if(index<0 || index>=this->childCount) return (MenuItem*)NULL;
	return this->childs[index];
}

MenuItem* MenuItem::addChild(MenuItem *item){
	MenuItem **copy = new MenuItem*[this->childCount+1];

	if(this->childs!=(MenuItem**)NULL){
		memcpy((void*)copy, this->childs, sizeof(MenuItem*)*this->childCount);
		delete[] this->childs;
	}

	item->setRoot(this->root);

	copy[this->childCount] = item;
	this->childCount++;
	this->childs = copy;

	return item;
}
MenuItem* MenuItem::addChild(const wchar_t *key, const wchar_t *text){
	MenuItem **copy = new MenuItem*[this->childCount+1];
	MenuItem *item = new MenuItem(key, text, this);

	if(this->childs!=(MenuItem**)NULL){
		memcpy((void*)copy, this->childs, sizeof(MenuItem*)*this->childCount);
		delete[] this->childs;
	}

	item->setRoot(this->root);

	copy[this->childCount] = item;
	this->childCount++;
	this->childs = copy;

	return item;
}
MenuItem* MenuItem::addChild(const wchar_t *key, const wchar_t *text, int index){
	if(index<0 || index>this->childCount) return (MenuItem*)NULL;

	MenuItem **copy = new MenuItem*[this->childCount+1];
	MenuItem *item = new MenuItem(key, text, this);

	if(index>0) memcpy(copy, this->childs, sizeof(MenuItem*)*index);
	if(index<this->childCount) memcpy(&copy[index+1], this->childs+index, sizeof(MenuItem*)*(this->childCount-index));

	if(this->childs!=(MenuItem**)NULL) delete[] this->childs;

	item->setRoot(this->root);

	copy[index] = item;
	this->childCount++;
	this->childs = copy;

	return item;
}
MenuItem* MenuItem::removeChild(int index){
	if(index<0 || index>=this->childCount) return this;

	MenuItem **copy = new MenuItem*[this->childCount-1];

	if(index>0) memcpy(copy, this->childs, sizeof(MenuItem*)*index);
	memcpy(copy+index, this->childs+index+1, sizeof(MenuItem*)*(this->childCount-index-1));

	delete[] this->childs;

	this->childCount--;
	this->childs = copy;

	return this;
}

const wchar_t* MenuItem::getKey(){
	return (const wchar_t*)this->key;
}

MenuItem* MenuItem::setText(const wchar_t *text){
	if(this->text!=(wchar_t*)NULL) delete[] this->text;

	int len = wcslen(text);
	this->text = new wchar_t[len+1];
	wcscpy_s(this->text, len+1, text);

	return this;
}
const wchar_t* MenuItem::getText(){
	return (const wchar_t*)this->text;
}

MenuItem* MenuItem::popupShow(int x, int y){
	HWND parentHWND = (HWND)NULL;
	if(this->root){
		GUIObject *parent = this->root->getParent();
		if(parent) parentHWND = parent->getHWND();
	}

	HMENU menu = this->buildMenu(TRUE);
		UINT ret = TrackPopupMenu(menu, TPM_WORKAREA | TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, x, y, 0, parentHWND, (RECT*)NULL);

		if(this->onMenuClick) this->onMenuClick->Event(this->root, (const wchar_t*)ret);
	DestroyMenu(menu);
	return this;
}
