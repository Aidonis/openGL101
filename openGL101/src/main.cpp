#define STB_IMAGE_IMPLEMENTATION

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
			app->GenerateGrid(5, 5);
			//app->createOpenGLBuffers(app->shapes);
		}
		app->Shutdown();
	}

	delete(app);
	return 0;
}