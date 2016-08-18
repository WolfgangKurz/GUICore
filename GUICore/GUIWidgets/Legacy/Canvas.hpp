/*
	GUIWidgets - Legacy - Canvas
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Canvas class
*/

#pragma once

namespace GUILegacy {
class Canvas : public GUIWidget{
private:
	int backColor;
	GUIImage *picture;
	GUIImage *image;

	GUIImage* image_new(int width, int height);
	void image_delete(GUIImage **image);

public:
	Canvas();
	Canvas(int x, int y, int width, int height);
	virtual ~Canvas();

	virtual Canvas* invalidate(BOOL immediately=FALSE);

	virtual Canvas* move_center();
	virtual Canvas* move(int x, int y);
	virtual Canvas* resize(int width, int height);

	virtual Canvas* show();
	virtual Canvas* hide();

	GUIImage* getImage();
	GUIImage* getPicture();

	Canvas* setImage(GUIImage *image);
	Canvas* setPicture(GUIImage *picture);
	Canvas* loadImage(const wchar_t *path);

	Canvas* setBackColor(int color);
	int getBackColor();

	virtual LRESULT handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};
}
