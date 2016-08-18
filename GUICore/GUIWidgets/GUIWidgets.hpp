/*
	GUIWidgets - GUIWidgets
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Import default widgets
*/

#pragma once

#include "GUIWidgetOpt.hpp"

#ifndef GUIWidgets_NoNeo
	#include "Neo/Button.hpp"
	#include "Neo/Statusbar.hpp"
#endif

#ifndef GUIWidgets_NoLegacy
	#include "Legacy/Button.hpp"
	#include "Legacy/TextBox.hpp"
	#include "Legacy/Canvas.hpp"
	#include "Legacy/Menu.hpp"
#endif
