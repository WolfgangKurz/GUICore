/*
	GUIWidgets - Legacy - Button
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Button class
*/

#pragma once

namespace GUILegacy {
class Button : public GUIWidget{
private:
	int backColor, textColor;
	WNDPROC btnProcPrev;
	HWND btnHWND;

public:
	Button();
	Button(int x, int y, int width, int height, int style=0);
	virtual ~Button();

	virtual Button* invalidate(BOOL immediately=FALSE);

	virtual Button* move_center();
	virtual Button* move(int x, int y);
	virtual Button* resize(int width, int height);

	virtual Button* setFont(const wchar_t *fontName, int fontSize, int weight);
	virtual Button* setFont(HFONT font);
	virtual HFONT getFont();

	virtual Button* show();
	virtual Button* hide();

	WNDPROC getButtonProc();

	Button* setBackColor(int color);
	int getBackColor();

	Button* setText(const wchar_t *text);
	const wchar_t* getText();

	GUIEventP onPaint; // Not used

	virtual LRESULT handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};
}
