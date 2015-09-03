#include <iostream>

#include "Application/Application.h"

Application* app;

int main() {
	
	app = new Application();
	if(app->Init() == ApplicationFail::NONE)
	{
		while (app->Update() == true)
		{
			app->Tick();
			app->Draw();
		}
		app->Shutdown();
	}

	delete(app);
	return 0;
}