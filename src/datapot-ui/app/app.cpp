#include "app.h"

void ImApp::init() {
    mainWindow = new MyMainWindow();
}

void ImApp::quit() {
    _DeletePointer(mainWindow);
}

void ImApp::tick() {
	mainWindow->render(0);
}
