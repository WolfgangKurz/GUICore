/*
	GUIWidgets - Neo - Statusbar
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Statusbar class
*/

#pragma once

namespace GUINeo {
class Statusbar : public GUIWidget {
private:
	BOOL resizable;
	int backColor, textColor;
	wchar_t *text;

public:
	Statusbar(int height);
	virtual ~Statusbar();

	virtual Statusbar* invalidate(BOOL immediately=FALSE);

	virtual Statusbar* move_center();
	virtual Statusbar* move(int x, int y);
	virtual Statusbar* resize(int width, int height);
	Statusbar* locate();

	virtual Statusbar* show();
	virtual Statusbar* hide();

	Statusbar* setResizable(BOOL resizable);
	BOOL getResizable();

	Statusbar* setBackColor(int color);
	int getBackColor();

	Statusbar* setTextColor(int color);
	int getTextColor();

	Statusbar* setText(const wchar_t *text);
	const wchar_t* getText();

	virtual LRESULT handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};
}
