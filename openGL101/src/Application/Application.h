#pragma once
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include <aie\Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include <string>

#include "../Planets/Planet.h"
#include "../Camera/Camera.h"
#include "../Camera/FlyCamera.h"

struct GLFWwindow;



enum ApplicationFail{
	NONE,
	GLFW_INIT,
	GLFW_CREATE_WINDOW,
	OGL_LOAD_FUNCTIONS
};

class Application{

	std::string name_string;
	int width, height;

	GLFWwindow* window;
	float elapsedTime, currentTime, previousTime, lagTime;
	glm::mat4 projection, view;

	const double TICK_PER_SEC_D_;

	Planet* earth;
	Camera* camera;

public:
	Application();
	Application(std::string set_name);
	Application(std::string set_name, int set_width, int set_height);
	~Application();

	ApplicationFail Init();
	void Shutdown();

	bool Update();
	void Tick();
	void Draw();

	virtual void onInit(){ };

	virtual void onTerm(){ };

	virtual void onStep(){ };

	virtual void onDraw(){ };

};