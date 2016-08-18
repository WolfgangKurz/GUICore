/*
	GUICore - GUIObject
	 The win32 GUI framework library.
	 author: WolfgangKurz

	Base class for windows and widgets
	A standard object unit of system
*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GUIValues.h"

class GUIObject {
private:
	void *onMessage;

protected:
	HFONT hFont;
	HWND hWnd;
	GUIObject *parent;
	UINT objFlag;

public:
	GUIObject(UINT flag=GUIOF_NONE, UINT style=WS_OVERLAPPEDWINDOW, UINT exstyle=0, HWND parent=HWND_DESKTOP);
	GUIObject(int x, int y, int width, int height, UINT flag=GUIOF_NONE, UINT style=WS_OVERLAPPEDWINDOW, UINT exstyle=0, HWND parent=HWND_DESKTOP);
	virtual ~GUIObject();

	HWND getHWND();
	virtual GUIObject* setParent(GUIObject *parent);
	virtual GUIObject* getParent();

	virtual GUIObject* setFont(const wchar_t *fontName, int fontSize, int weight);
	virtual GUIObject* setFont(HFONT font);
	virtual HFONT getFont();

	virtual GUIObject* invalidate(BOOL immediately=FALSE);

	virtual GUIObject* show(int show=5);
	virtual GUIObject* hide();

	virtual GUIObject* move_center();
	virtual GUIObject* move(int x, int y);
	virtual GUIObject* resize(int width, int height);

	virtual int getX();
	virtual int getY();
	virtual int getWidth();
	virtual int getHeight();
	virtual int getInnerWidth();
	virtual int getInnerHeight();

	virtual LRESULT handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#ifdef GUIObject_DismissOnMessage
	GUIObject* setOnMessage(void *onMessage);
#elif defined(__GNUC__)
	GUIObject* setOnMessage(void *onMessage)
		__attribute__((deprecated("Caution. Do not use this method as you can. Parameter is GUIMessageEvent, Event have to return TRUE when processed.")));
#elif defined(_MSC_VER)
	__declspec(deprecated("Caution. Do not use this method as you can. Parameter is GUIMessageEvent, Event have to return TRUE when processed."))
		GUIObject* setOnMessage(void *onMessage);
#endif

	void* getOnMessage();

	/*
	 -- Important --
	 When you use this, you have to allocate new memory.
	 GUIObject frees memory automatically.
	*/
	void *extra;
};
