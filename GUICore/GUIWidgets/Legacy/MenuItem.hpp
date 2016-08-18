/*
	GUIWidgets - MenuItem
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Item of Menu class
	need to use Menu
*/

#pragma once

#include "MenuItem.hpp"

namespace GUILegacy {
class MenuItem {
private:
	MenuItem *parent;
	GUIWidget *root;
	BOOL enabled;

	int childCount;
	MenuItem **childs;

	wchar_t *key;
	wchar_t *text;

public:
	MenuItem(const wchar_t *key, const wchar_t *text, MenuItem *parent);
	~MenuItem();

	MenuItem *clone(MenuItem *parent = (MenuItem*)NULL);

	MenuItem* setEnabled(BOOL enabled);
	BOOL getEnabled();

	MenuItem* setRoot(GUIWidget *root);
	GUIWidget* getRoot();
	MenuItem* getParent();

	HMENU buildMenu(BOOL popup);
	void stackMenu(HMENU baseMenu);

	int getChildCount();
	MenuItem* getChild(int index);

	MenuItem* addChild(MenuItem *item);
	MenuItem* addChild(const wchar_t *key, const wchar_t *text);
	MenuItem* addChild(const wchar_t *key, const wchar_t *text, int index);
	MenuItem* removeChild(int index);

	const wchar_t* getKey();

	MenuItem* setText(const wchar_t *text);
	const wchar_t* getText();

	MenuItem* popupShow(int x, int y);

	GUIMenuEventP onMenuClick; // GUIMenuEvent::Event(GUIObject *sender, const wchar_t *key)
};
}
