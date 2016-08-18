/*
	GUIWidgets - Legacy - TextBox
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	TextBox class
*/

#pragma once

namespace GUILegacy {
class TextBox : public GUIWidget{
private:
	int backColor, textColor;
	WNDPROC editProcPrev;;
	HWND editHWND;

public:
	TextBox();
	TextBox(int x, int y, int width, int height, int style=0);
	virtual ~TextBox();

	virtual TextBox* invalidate(BOOL immediately=FALSE);

	virtual TextBox* move_center();
	virtual TextBox* move(int x, int y);
	virtual TextBox* resize(int width, int height);

	virtual TextBox* setFont(const wchar_t *fontName, int fontSize, int weight);
	virtual TextBox* setFont(HFONT font);
	virtual HFONT getFont();

	virtual TextBox* show();
	virtual TextBox* hide();

	WNDPROC getEditProc();

	TextBox* setBackColor(int color);
	int getBackColor();

	TextBox* setTextColor(int color);
	int getTextColor();

	TextBox* setText(const wchar_t *text);
	const wchar_t* getText();

	GUIEventP onChange; // GUIEvent::Event(GUIObject *sender)
	GUIEventP onPaint; // Not used

	virtual LRESULT handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};
}
