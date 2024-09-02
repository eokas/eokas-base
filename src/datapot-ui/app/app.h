#pragma once

#include "./window.h"
using namespace eokas::datapot;

struct ImApp
{
	void init();
	void quit();
	void tick();
    
    struct MyMainWindow* mainWindow;
};