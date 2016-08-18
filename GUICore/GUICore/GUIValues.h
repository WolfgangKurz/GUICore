/*
	GUICore - GUIValues
	 The win32 GUI framework library.
	 author: WolfgangKurz

	Values for GUICore
*/

#pragma once

#define GUIObjectDefault ((int)0x80000000)
#define GUIWindowDefault GUIObjectDefault

typedef enum GUIMouse {
	GUIMouseNone = 0,
	GUIMouseLeft,
	GUIMouseRight,
	GUIMouseLeftRight,
	GUIMouseMiddle,
	GUIMouseLeftMiddle,
	GUIMouseRightMiddle,
	GUIMouseLeftRightMiddle,
	GUIMouseForceDWORD = 0x7fffffff
} GUIMouse;

typedef enum GUIShift {
	GUIShiftNone = 0,
	GUIShiftControl,
	GUIShiftShift,
	GUIShiftForceDWORD = 0x7fffffff
} GUIShift;

typedef struct GUIImage {
	HDC hDC;
	HBITMAP hBmp, hBmpOld;
	int width, height;
} GUIImage;

typedef enum GUITextAlign {
	GUITextAlignLeft = 0,
	GUITextAlignRight,
	GUITextAlignMiddle,
	GUITextAlignDWORD = 0x7fffffff
} GUITextAlign;

/*
 GUIObject Flags
*/
#define GUIOF_NONE      0x0000
#define GUIOF_SHADOW    0x0001 // Drop shadow
#define GUIOF_ADJUST    0x0002 // Adjust size as inner
