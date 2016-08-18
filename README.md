![GUICore.png](https://github.com/WolfgangKurz/GUICore/raw/master/GUICore.png)
# GUICore #

**Create your window-based programs easily!**

GUICore is C++ based library, so you can build your application very easily, and quickly!


# Installing #

**GUICore is Windows libarary**

* Download
* Compile
* Include to your project
* Done!

# Documentation #

Not yet.


# Extra Widgets #

Menu - Unstable, deprecated


# Sample #

**Create an empty window**
```
#!c++
#include "GUICore/GUICore.hpp"

class MainUnloadEvent : public GUIEvent {
	virtual long Event(GUIObject *){
		return GUICore::quit();
	}
};

int main(void){
	GUICore::initialize();

	GUIWindow *window = new GUIWindow(0, 0, 200, 160);
	window->show()
		->onUnload = new MainUnloadEvent();

	GUICore::loop();
	delete window;

	GUICore::uninitialize();
	return 0;
}
```
