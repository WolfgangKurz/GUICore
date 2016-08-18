/*
	GUIWidgets - Legacy - Canvas
	 The default widgets for use in GUICore.
	 author: WolfgangKurz

	Canvas class
*/

#include "../GUIWidgetOpt.hpp"

#include "Canvas.hpp"

#include <stdio.h>
#include <io.h>
#include <OleCtl.h>

using namespace GUILegacy;

GUIImage* Canvas::image_new(int width, int height){
	HDC gDC;
	GUIImage *image = new GUIImage();
	memset(image, 0, sizeof(GUIImage));

	gDC = GetDC(HWND_DESKTOP);
		image->hDC = CreateCompatibleDC(gDC);
		image->hBmpOld = (HBITMAP)SelectObject(image->hDC, CreateCompatibleBitmap(gDC, width, height));
		image->width = width;
		image->height = height;
	ReleaseDC(HWND_DESKTOP, gDC);

	return image;
}
void Canvas::image_delete(GUIImage **image){
	if(image==(GUIImage**)NULL) return;
	if(*image==(GUIImage*)NULL) return;
	delete (GUIImage*)(*image);
	*image = (GUIImage*)NULL;
}

Canvas::Canvas() : GUIWidget(0, 0, 0, 0){
	this->picture = (GUIImage*)NULL;
	this->image = (GUIImage*)NULL;

	this->backColor = 0xffffff;
}
Canvas::Canvas(int x, int y, int width, int height) : GUIWidget(x, y, width, height){
	this->picture = (GUIImage*)NULL;
	this->image = (GUIImage*)NULL;

	this->backColor = 0xffffff;

	this->image = this->image_new(width, height);
	BitBlt(this->image->hDC, 0, 0, width, height, this->image->hDC, 0, 0, WHITENESS);
}
Canvas::~Canvas(){
	this->image_delete(&this->image);
	this->image_delete(&this->picture);
}

Canvas* Canvas::invalidate(BOOL immediately){
	GUIWidget::invalidate(immediately);
	return this;
}

Canvas* Canvas::move_center(){
	GUIWidget::move_center();
	return this;
}
Canvas* Canvas::move(int x, int y){
	GUIWidget::move(x, y);
	return this;
}
Canvas* Canvas::resize(int width, int height){
	GUIWidget::resize(width, height);
	
	this->image_delete(&this->image);

	this->image = this->image_new(width, height);
	BitBlt(this->image->hDC, 0, 0, this->picture->width, this->picture->height, this->picture->hDC, 0, 0, SRCCOPY);

	return this;
}

Canvas* Canvas::show(){
	GUIWidget::show();
	return this;
}
Canvas* Canvas::hide(){
	GUIWidget::hide();
	return this;
}

GUIImage* Canvas::getImage(){
	return this->image;
}
GUIImage* Canvas::getPicture(){
	return this->picture;
}

Canvas* Canvas::setImage(GUIImage *image){
	if(image==(GUIImage*)NULL){
		this->image_delete(&this->image);
	}else{
		GUIImage *image_new = this->image_new(image->width, image->height);
		BitBlt(image_new->hDC, 0, 0, image_new->width, image_new->height, image->hDC, 0, 0, SRCCOPY);

		this->image_delete(&this->image);
		this->image = image_new;
	}
	this->invalidate();
	return this;
}
Canvas* Canvas::setPicture(GUIImage *picture){
	if(picture==(GUIImage*)NULL){
		this->image_delete(&this->picture);
	}else{
		GUIImage *picture_new = this->image_new(picture->width, picture->height);
		BitBlt(picture_new->hDC, 0, 0, picture_new->width, picture_new->height, picture->hDC, 0, 0, SRCCOPY);

		this->image_delete(&this->picture);
		this->picture = picture_new;
	}
	this->invalidate();
	return this;
}

Canvas* Canvas::loadImage(const wchar_t *path){
	FILE *fp;
	long size;
	LPPICTURE picture;
	HRESULT res;

	_wfopen_s(&fp, path, L"rb");
	if(fp==(FILE*)NULL) return this;

	size = _filelength(_fileno(fp));

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, size);
	if(hGlobal==(HGLOBAL)NULL){
		fclose(fp);
		return this;
	}

	void *ptr = GlobalLock(hGlobal);
	if(ptr==(void*)NULL){
		GlobalFree(hGlobal);
		fclose(fp);
		return this;
	}

	fread(ptr, 1, size, fp);
	fclose(fp);

	GlobalUnlock(hGlobal);

	LPSTREAM pstm = (LPSTREAM)NULL;
	res = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
	if( res!=S_OK || pstm==(LPSTREAM)NULL ){
		return this;
	}

	res = OleLoadPicture(pstm, size, FALSE, IID_IPicture, (void**)&picture);
	if( res!=S_OK || picture==(LPPICTURE)NULL ){
		pstm->Release();
		return this;
	}

	pstm->Release();
	GlobalFree(hGlobal);

	OLE_XSIZE_HIMETRIC width = 0;
	OLE_YSIZE_HIMETRIC height = 0;

	HDC gDC = GetDC(HWND_DESKTOP), hDC = CreateCompatibleDC(gDC);
	ReleaseDC(HWND_DESKTOP, gDC);

	picture->get_Width(&width);
	picture->get_Height(&height);
	width = MulDiv(width, GetDeviceCaps(hDC, LOGPIXELSX), 2540);
	height = MulDiv(height, GetDeviceCaps(hDC, LOGPIXELSY), 2540);

	HBITMAP hBmp, hOld;
	picture->get_Handle((unsigned int*)&hBmp);
		hBmp = (HBITMAP)CopyImage(hBmp, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
	picture->Release();

	hOld = (HBITMAP)SelectObject(hDC, hBmp);
		GUIImage *picture_new = this->image_new((int)width, (int)height);
		BitBlt(picture_new->hDC, 0, 0, picture_new->width, picture_new->height, hDC, 0, 0, SRCCOPY);

		this->image_delete(&this->picture);
		this->picture = picture_new;
	DeleteObject(SelectObject(hDC, hOld));
	DeleteDC(hDC);

	this->setBackColor(this->backColor);
	this->invalidate();
	return this;
}

Canvas* Canvas::setBackColor(int color){
	RECT rc = {0, 0, this->image->width, this->image->height};
	HBRUSH hbr = CreateSolidBrush(color);
		FillRect(this->image->hDC, &rc, hbr);
		if(this->picture!=(GUIImage*)NULL)
			BitBlt(this->image->hDC, 0, 0, this->picture->width, this->picture->height, this->picture->hDC, 0, 0, SRCCOPY);

		InvalidateRect(hWnd, NULL, FALSE);
	DeleteObject(hbr);

	this->backColor = color;
	this->invalidate();
	return this;
}
int Canvas::getBackColor(){
	return this->backColor;
}

LRESULT Canvas::handleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	switch(Msg){
		case WM_PAINT:{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
				BitBlt(hDC, 0, 0, this->image->width, this->image->height, this->image->hDC, 0, 0, SRCCOPY);

				if(this->onPaint) this->onPaint->Event(this, hDC);
			EndPaint(hWnd, &ps);
			return 0;}
	}
	return GUIWidget::handleMessage(hWnd, Msg, wParam, lParam);
}
