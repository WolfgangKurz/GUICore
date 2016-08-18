/*
	GUICore - GUIWindow
	 The win32 GUI framework library.
	 author: WolfgangKurz

	Base class for windows
	A standard unit of parent windows
*/

#pragma once

#include "GUIObject.hpp"
#include "GUIWidget.hpp"
#include "GUIEvent.hpp"
#include "GUIEvents.hpp"

#include "GUIValues.h"

#include "../GUIWidgets/Legacy/Menu.hpp"

class GUIWindow : public GUIObject {
protected:
	friend class GUILegacy::Menu;

	void *widgets;
	GUIMenuEventP onMenuClick; // GUIMenuEvent::Event(GUIObject *sender, const wchar_t *key)

public:
	GUIWindow(UINT flag=GUIOF_NONE, UINT style=WS_OVERLAPPEDWINDOW, UINT exstyle=0, HWND parent=HWND_DESKTOP);
	GUIWindow(int x, int y, int width, int height, UINT flag=GUIOF_NONE, UINT style=WS_OVERLAPPEDWINDOW, UINT exstyle=0, HWND parent=HWND_DESKTOP);
	virtual ~GUIWindow();

	virtual GUIWindow* setParent(GUIObject *parent);

	virtual GUIWindow* setFont(const wchar_t *fontName, int fontSize, int weight);
	virtual GUIWindow* setFont(HFONT font);

	virtual GUIWindow* invalidate(BOOL immediately=FALSE);

	virtual GUIWindow* show(int show=5);
	virtual GUIWindow* hide();

	virtual GUIWindow* move_center();
	virtual GUIWindow* move(int x, int y);
	virtual GUIWindow* resize(int width, int height);

	virtual GUIWindow* setText(const wchar_t *text);
	virtual const wchar_t* getText();

	GUIWidget* addControl(wchar_t *name, GUIWidget *widget);
	GUIWidget* getControl(wchar_t *name);
	const wchar_t* getControl(GUIObject *object);
	int removeControl(LPWSTR name);

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
