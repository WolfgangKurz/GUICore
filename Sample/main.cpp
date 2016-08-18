#define GUIObject_DismissOnMessage // Dismiss "Caution" warning on setOnMessage method
#include "GUICore.hpp"
#include "GUIWidgets.hpp"

#include "WidgetLegacyMenu.hpp"

class MainUnloadEvent : public GUIEvent {
	virtual long Event(GUIObject *){
		return GUICore::quit();
	}
};
class MainPaintEvent : public GUIPaintEvent {
public:
	virtual long Event(GUIObject *sender, HDC hDC){
		RECT rc;
		GetClientRect(sender->getHWND(), &rc);

		HBRUSH hbr = CreateSolidBrush(0xCC6633);
			FillRect(hDC, &rc, hbr);
		DeleteObject(hbr);

		BitBlt(hDC, rc.left+2, rc.top+44, rc.right-2-(rc.left+2), rc.bottom-2-(rc.top+44), hDC, 0, 0, WHITENESS);

		SetTextColor(hDC, 0xFFFFFF);
		SetBkMode(hDC, TRANSPARENT);

		const wchar_t *text = ((GUIWindow*)sender)->getText();
			SetRect(&rc, rc.left, rc.top+2, rc.right, rc.top+2+20);
			DrawText(hDC, text, wcslen(text), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		delete[] text;
		return 0;
	}
};
class MainResizeEvent : public GUIResizeEvent {
public:
	virtual long Event(GUIObject *sender, int, int){
		GUIWindow *window = (GUIWindow*)sender;
		WidgetLegacyMenu *menu = (WidgetLegacyMenu*)window->getControl(L"Menu");

		((Statusbar*)window->getControl(L"Statusbar1"))->locate();
		menu->resize(sender->getInnerWidth()-4, menu->getHeight());
		return 0;
	}
};
class Button1ClickEvent : public GUIMouseEvent {
public:
	virtual long Event(GUIObject *sender, GUIMouse, GUIShift, int, int){
		GUIWindow *window = GUICore::parentWindow(sender);
		const wchar_t *text = ((TextBox*)window->getControl(L"TextBox1"))->getText();
		MessageBox(sender->getHWND(), text, L"GUICore", MB_ICONINFORMATION);
		delete[] text;
		return 0;
	};
};
class ButtonRGBClickEvent : public GUIMouseEvent {
public:
	virtual long Event(GUIObject *sender, GUIMouse, GUIShift, int, int){
		GUIWindow *window = GUICore::parentWindow(sender);
		LPCWSTR name = window->getControl(sender);
		int bcolor = 0xFFFFFF;

		if( wcscmp(name, L"btnRed")==0 ){
			bcolor = 0x0000FF;
		}else if( wcscmp(name, L"btnGreen")==0 ){
			bcolor = 0x008000;
		}else if( wcscmp(name, L"btnBlue")==0 ){
			bcolor = 0xFF0000;
		}else if( wcscmp(name, L"btnWhite")==0 ){
			bcolor = 0xFFFFFF;
		}
		window->setText(name);

		((Canvas*)window->getControl(L"Canvas1"))
			->setBackColor(bcolor);
		return 0;
	};
};
class MenuClick : public GUIMenuEvent {
public:
	virtual long Event(GUIObject *sender, const wchar_t *key){
		if(key==(const wchar_t*)NULL) return 0;

		if( wcscmp(key, L"mF_msg")==0 ){
			HWND hWnd = (sender!=(GUIObject*)NULL ? sender->getHWND() : HWND_DESKTOP);
			MessageBox(hWnd, key, key, MB_ICONINFORMATION);
		}else if( wcscmp(key, L"mF_quit")==0 ){
			GUICore::quit();
		}
		return 0;
	}
};

class MainWindowProc : public GUIMessageEvent {
public:
	virtual long Event(GUIObject *sender, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *ret){
		switch(Msg){
			case WM_NCCALCSIZE:
				if(wParam==TRUE){
					NCCALCSIZE_PARAMS *p = (NCCALCSIZE_PARAMS*)lParam;
					if( GetWindowLong(sender->getHWND(), GWL_STYLE) & WS_MAXIMIZE ){
						p->rgrc[0].left   += 8;
						p->rgrc[0].top    += 8;
						p->rgrc[0].right  -= 8;
						p->rgrc[0].bottom -= 8;
					}
				}else{
					RECT *rc = (RECT*)lParam;
					if( GetWindowLong(sender->getHWND(), GWL_STYLE) & WS_MAXIMIZE ){
						rc->left   += 8;
						rc->top    += 8;
						rc->right  -= 8;
						rc->bottom -= 8;
					}
				}
				*ret = (LRESULT)0;
				return TRUE;
			case WM_NCHITTEST:{
				POINT pt = {LOWORD(lParam), HIWORD(lParam)};
				RECT rc;

				GetClientRect(sender->getHWND(), &rc);
				ScreenToClient(sender->getHWND(), &pt);

				if(pt.x<4 && pt.y<4)                            *ret = (LRESULT)HTTOPLEFT;
				else if(pt.x>=rc.right-4 && pt.y<4)             *ret = (LRESULT)HTTOPRIGHT;
				else if(pt.x<4 && pt.y>=rc.bottom-4)            *ret = (LRESULT)HTBOTTOMLEFT;
				else if(pt.x>=rc.right-4 && pt.y>=rc.bottom-4)  *ret = (LRESULT)HTBOTTOMRIGHT;
				else if(pt.x<4)             *ret = (LRESULT)HTLEFT;
				else if(pt.x>=rc.right-4)   *ret = (LRESULT)HTRIGHT;
				else if(pt.y<4)             *ret = (LRESULT)HTTOP;
				else if(pt.y>=rc.bottom-4)  *ret = (LRESULT)HTBOTTOM;
				else if(pt.y<20)            *ret = (LRESULT)HTCAPTION;
				else  *ret = (LRESULT)HTCLIENT;

				return 1;}
		}
		return FALSE;
	}
};

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

int main(void){
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GUICore::consoleHide();

	GUICore::initialize();
	{
		GUIWindow *window = new GUIWindow(GUIWindowDefault, GUIWindowDefault, 524, 680);
		window->move_center()
			->setFont(L"맑은 고딕", 12, 400);
		window->onUnload = new MainUnloadEvent();
		window->onPaint = new MainPaintEvent();
		window->onResize = new MainResizeEvent();
		window->setOnMessage(new MainWindowProc());
		SetWindowRgn(window->getHWND(), (HRGN)NULL, TRUE);

		Statusbar *statusbar = new Statusbar(23);
		statusbar->setBackColor(0xCC6633)
			->setTextColor(0xFFFFFF)
			->setText(L"스테이터스바 테스트")
			->setResizable(TRUE);
		
		TextBox *textbox = new TextBox(12, 60, 220, 24, ES_AUTOHSCROLL);
		textbox->setBackColor(0xFFFFFF)
			->setTextColor(0x000000);
		
		Button *button = new Button(12, 88, 128, 24);
		button->setBackColor(0xCC6633)
			->setActiveBackColor(0x994422)
			->setTextColor(0xFFFFFF)
			->setHoverColor(0xDD8866)
			->setText(L"텍스트박스 출력")
			->onClick = new Button1ClickEvent();

		Button *buttonR = new Button(12, 128, 40, 24);
		Button *buttonG = new Button(56, 128, 40, 24);
		Button *buttonB = new Button(100, 128, 40, 24);
		Button *buttonW = new Button(144, 128, 40, 24);
		buttonR->setBackColor(0xCC6633)->setActiveBackColor(0x994422)
			->setTextColor(0xFFFFFF)->setText(L"R")
			->onClick = new ButtonRGBClickEvent();
		buttonG->setBackColor(0xCC6633)->setActiveBackColor(0x994422)
			->setTextColor(0xFFFFFF)->setText(L"G")
			->onClick = new ButtonRGBClickEvent();
		buttonB->setBackColor(0xCC6633)->setActiveBackColor(0x994422)
			->setTextColor(0xFFFFFF)->setText(L"B")
			->onClick = new ButtonRGBClickEvent();
		buttonW->setBackColor(0xCC6633)->setActiveBackColor(0x994422)
			->setTextColor(0xFFFFFF)->setText(L"W")
			->onClick = new ButtonRGBClickEvent();

		Canvas *canvas = new Canvas(12, 160, 500, 512);
		canvas->loadImage(L"image.jpg");

		window->addControl(L"Button1", button);

		window->addControl(L"btnRed", buttonR);
		window->addControl(L"btnGreen", buttonG);
		window->addControl(L"btnBlue", buttonB);
		window->addControl(L"btnWhite", buttonW);

		window->addControl(L"TextBox1", textbox);
		window->addControl(L"Canvas1", canvas);
		window->addControl(L"Statusbar1", statusbar);

		WidgetLegacyMenu *menu = new WidgetLegacyMenu(2, 24, 0, 20);
		menu->setBackColor(0xCC6633)
			->setActiveBackColor(0x994422)
			->setTextColor(0xFFFFFF)
			->setHoverColor(0xDD8866)
			->onMenuClick = new MenuClick();

		menu->addChild(L"mF", L"파일 (&F)");
		menu->getChild(0)->addChild(L"mF_msg", L"메세지 박스 (&M)");
		menu->getChild(0)->addChild(L"mF_disabled", L"사용 불가 메뉴 (&D)")->setEnabled(FALSE);
		menu->getChild(0)->addChild(L"", L"-");
		menu->getChild(0)->addChild(L"mF_quit", L"종료 (&Q)");

		window->addControl(L"Menu", menu);

		window->show();

		GUICore::loop();
		delete window;
	}
	GUICore::uninitialize();

	return 0;
}
