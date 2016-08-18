/*
	GUICore - GUIWidget
	 The win32 GUI framework library.
	 author: WolfgangKurz

	Base class for widgets
	A standard unit of child controls
*/

#pragma once

#include "GUIObject.hpp"
#include "GUIEvent.hpp"
#include "GUIEvents.hpp"

#define GUIWidgetDefault GUIObjectDefault

class GUIWidget : public GUIObject {
public:
	GUIWidget(int x, int y, int width, int height, UINT style=0, UINT exstyle=0);
	virtual ~GUIWidget();

	virtual GUIWidget* setParent(GUIObject *parent);

	virtual GUIWidget* setFont(const wchar_t *fontName, int fontSize, int weight);
	virtual GUIWidget* setFont(HFONT font);

	virtual GUIWidget* invalidate(BOOL immediately=FALSE);

	virtual GUIWidget* show(int show=5);
	virtual GUIWidget* hide();

	virtual GUIWidget* move_center();
	virtual GUIWidget* move(int x, int y);
	virtual GUIWidget* resize(int width, int height);

	virtual LRESULT handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	GUIEventP onUnload; // GUIEvent::Event(GUIObject *sender)
	GUIFocusEventP onFocus, onKillFocus; // GUIFocusEvent::Event(GUIObject *sender, HWND focusHWND)
	GUIMouseEventP onClick, onDblClick; // GUIMouseEvent::Event(GUIObject *sender, GUIMouse button, GUIShift shift, int x, int y)
	GUIMouseEventP onMouseDown, onMouseMove, onMouseUp; // GUIMouseEvent::Event(GUIObject *sender, GUIMouse button, GUIShift shift, int x, int y)
	GUIEventP onMouseHover, onMouseLeave; // GUIEvent::Event(GUIObject *sender)
	GUIKeyEventP onKeyDown, onKeyPress, onKeyUp; // GUIKeyEvent::Event(GUIObject *sender, int key, GUIShift shift)
	GUIPaintEventP onPaint; // GUIPaintEvent::Event(GUIObject *sender, HDC hDC)
	GUIMoveEventP onMove; // GUIMoveEvent::Event(GUIObject *sender, int x, int y)
	GUIResizeEventP onResize; // GUIResizeEvent::Event(GUIObject *sender, int width, int height)
};
