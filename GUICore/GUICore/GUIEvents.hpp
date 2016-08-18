/*
	GUICore - GUIEvents
	 The win32 GUI framework library.
	 author: WolfgangKurz

	Extended event classes for default GUI objects
*/

#pragma once

#include "GUIEvent.hpp"
#include "GUIValues.h"

// virtual long Event(GUIObject *sender, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result)
typedef class GUIMessageEvent : public GUIEvent {
public:
	virtual long Event(GUIObject *sender, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *result){
		sender, Msg, wParam, lParam, result; return FALSE;
	};
} *GUIMessageEventP;

// virtual long Event(GUIObject *sender, HWND focusHWND)
typedef class GUIFocusEvent : public GUIEvent {
public:
	virtual long Event(GUIObject *sender, HWND focusHWND){
		sender, focusHWND; return 0;
	};
} *GUIFocusEventP;

// virtual long Event(GUIObject *sender, GUIMouse button, GUIShift shift, int x, int y)
typedef class GUIMouseEvent : public GUIEvent {
public:
	virtual long Event(GUIObject *sender, GUIMouse button, GUIShift shift, int x, int y){
		sender, button, shift, x, y; return 0;
	};
} *GUIMouseEventP;

// virtual long Event(GUIObject *sender, int key, GUIShift shift)
typedef class GUIKeyEvent : public GUIEvent {
public:
	virtual long Event(GUIObject *sender, int key, GUIShift shift){
		sender, key, shift; return 0;
	};
} *GUIKeyEventP;

// virtual long Event(GUIObject *sender, HDC hDC)
typedef class GUIPaintEvent : public GUIEvent {
public:
	virtual long Event(GUIObject *sender, HDC hDC){
		sender, hDC; return 0;
	};
} *GUIPaintEventP;

// virtual long Event(GUIObject *sender, int x, int y)
typedef class GUIMoveEvent : public GUIEvent {
public:
	virtual long Event(GUIObject *sender, int x, int y){
		sender, x, y; return 0;
	};
} *GUIMoveEventP;

// virtual long Event(GUIObject *sender, int width, int height)
typedef class GUIResizeEvent : public GUIEvent {
public:
	virtual long Event(GUIObject *sender, int width, int height){
		sender, width, height; return 0;
	};
} *GUIResizeEventP;

// virtual long Event(GUIObject *sender, const wchar_t *key)
typedef class GUIMenuEvent : public GUIEvent {
public:
	virtual long Event(GUIObject *sender, const wchar_t *key){
		sender, key; return 0;
	};
} *GUIMenuEventP;
