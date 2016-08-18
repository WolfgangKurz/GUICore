/*
	GUIWidgets - Menu
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Menu class, require Button class
*/

#pragma once

#include "MenuItem.hpp"

namespace GUILegacy {
class Menu : public GUIWidget {
private:
	MenuItem *root;
	HMENU prevMenu;

public:
	Menu();
	Menu(MenuItem *menu);
	virtual ~Menu();

	virtual Menu* setParent(GUIObject *parent);

	virtual Menu* show();
	virtual Menu* hide();

	int getChildCount();
	MenuItem* getChild(int index);

	MenuItem* addChild(const wchar_t *key, const wchar_t *text);
	MenuItem* addChild(const wchar_t *key, const wchar_t *text, int index);
	Menu* removeChild(int index);

	Menu* popupShow(int x, int y);

	GUIMenuEventP onMenuClick; // GUIMenuEvent::Event(GUIObject *sender, const wchar_t *key)
};
}
