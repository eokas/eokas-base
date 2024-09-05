#pragma once

#include "./logic.h"
#include "./logic.h"
#include "./views.h"
using namespace eokas::datapot;

struct ImApp
{
	void init();
	void quit();
	void tick();
    
    MyMainWindow* mainWindow;
};