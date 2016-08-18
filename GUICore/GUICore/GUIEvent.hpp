/*
	GUICore - GUIEvent
	 The win32 GUI framework library.
	 author: WolfgangKurz

	Base event class for GUICore system
*/

#pragma once

#include "GUIObject.hpp"

typedef class GUIEvent {
public:
	virtual long Event(GUIObject *sender){
		sender; return 0;
	};
} *GUIEventP;
