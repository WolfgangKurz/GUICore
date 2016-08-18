/*
	GUIWidgets - Menu
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Menu class, require Button class
*/

#pragma once

#include "../Legacy/MenuItem.hpp"

namespace GUINeo {
	using namespace GUILegacy;

class WidgetMenu : public GUIWidget {
private:
	int backColor, activeBackColor;
	int hoverColor, textColor;
	int index, state;

	MenuItem *root;

public:
	WidgetMenu(int x, int y, int width, int height);
	virtual ~WidgetMenu();

	virtual WidgetMenu* setFont(const wchar_t *fontName, int fontSize, int weight);
	virtual WidgetMenu* setFont(HFONT font);

	virtual WidgetMenu* invalidate(BOOL immediately=FALSE);

	virtual WidgetMenu* move_center();
	virtual WidgetMenu* move(int x, int y);
	virtual WidgetMenu* resize(int width, int height);
	WidgetMenu* locate();

	virtual WidgetMenu* show();
	virtual WidgetMenu* hide();

	WidgetMenu* setBackColor(int color);
	int getBackColor();

	WidgetMenu* setActiveBackColor(int color);
	int getActiveBackColor();

	WidgetMenu* setHoverColor(int color);
	int getHoverColor();

	WidgetMenu* setTextColor(int color);
	int getTextColor();

	int getChildCount();
	MenuItem* getChild(int index);

	MenuItem* addChild(const wchar_t *key, const wchar_t *text);
	MenuItem* addChild(const wchar_t *key, const wchar_t *text, int index);
	WidgetMenu* removeChild(int index);

	WidgetMenu* popupShow(int x, int y);

	virtual LRESULT handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	GUIMenuEventP onMenuClick; // GUIMenuEvent::Event(GUIObject *sender, const wchar_t *key)
};
}
