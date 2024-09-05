#include "app.h"

void ImApp::init() {
    mainWindow = new MyMainWindow();
    Logic::instance().mainWindow = mainWindow;
}

void ImApp::quit() {
    Logic::instance().mainWindow = nullptr;
    _DeletePointer(mainWindow);
}

void ImApp::tick() {
	mainWindow->render(0);
}
