/*
	GUIWidgets - Neo - Button
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Button class
*/

#pragma once

namespace GUINeo {
class Button : public GUIWidget {
private:
	int backColor, activeBackColor;
	int hoverColor, textColor;
	int enabled, state;
	wchar_t *text;
	GUITextAlign align;

public:
	Button();
	Button(int x, int y, int width, int height);
	virtual ~Button();

	virtual Button* invalidate(BOOL immediately=FALSE);

	virtual Button* move_center();
	virtual Button* move(int x, int y);
	virtual Button* resize(int width, int height);

	virtual Button* show();
	virtual Button* hide();

	Button* setEnabled(int enabled);
	int getEnabled();

	Button* setBackColor(int color);
	int getBackColor();

	Button* setActiveBackColor(int color);
	int getActiveBackColor();

	Button* setHoverColor(int color);
	int getHoverColor();

	Button* setTextColor(int color);
	int getTextColor();

	Button* setText(const wchar_t *text);
	const wchar_t* getText();

	Button* setAlign(GUITextAlign align);
	GUITextAlign getAlign();

	virtual LRESULT handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};
}
