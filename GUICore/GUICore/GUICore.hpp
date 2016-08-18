/*
	GUICore - GUICore
	 The win32 GUI framework library.
	 author: WolfgangKurz

	Core class
	Contains many static utility methods
*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GUIObject.hpp"
#include "GUIWindow.hpp"
#include "GUIWidget.hpp"
#include "GUIEvent.hpp"
#include "GUIValues.h"

class GUICore {
private:
public:
	GUICore();
	~GUICore();

	static void initialize();
	static void uninitialize();

	static void consoleHide();
	static void consoleShow();

	static GUIObject* rootObject(GUIObject *object);
	static GUIWindow* parentWindow(GUIObject *object);

	static int loop(BOOL peek = FALSE, HACCEL accel = (HACCEL)NULL);
	static int quit();
};
