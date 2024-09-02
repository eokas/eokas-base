#pragma once

#include "./models.h"
#include "./controllers.h"
#include "./views.h"
using namespace eokas::datapot;

struct ImApp
{
	void init();
	void quit();
	void tick();
    
    MyMainWindow* mainWindow;
};