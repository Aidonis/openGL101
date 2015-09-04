#include "Application.h"

Application::Application() : Application("") {}

Application::Application(std::string set_name) : Application(set_name, 1280, 720) {}

Application::Application(std::string set_name, int set_width, int set_height) : TICK_PER_SEC_D_(1.0f / 60.0f) {
	name_string = set_name;
	width = set_width;
	height = set_height;
	window = nullptr;
	camera = nullptr;

	view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	currentTime = 0.0f;
	elapsedTime = 0.0f;
	lagTime = 0.0f;
	previousTime = 0.0f;
}

Application::~Application() {}

ApplicationFail Application::Init() {
	if (glfwInit() == false) {
		return ApplicationFail::GLFW_INIT;
	}

	window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		return ApplicationFail::GLFW_CREATE_WINDOW;
	}

	glfwMakeContextCurrent(window);

	//Camera
	camera = new FlyCamera(0.01f);
	camera->SetPerspective(glm::pi<float>() * 0.25f, SIXTEEN_NINE, 0.1f, 1000.f);
	camera->SetLookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));


	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return ApplicationFail::OGL_LOAD_FUNCTIONS;
	}

	Gizmos::create();



	//Set Clear Screen color
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	previousTime = glfwGetTime();

	earth = new Planet();
	earth->setPosition(glm::vec3(0, 2, 0));
	earth->setColor(glm::vec4(.5f, 0, .8f, 1));

	return ApplicationFail::NONE;
}

void Application::Shutdown(){
	delete(earth);
	delete(camera);
	Gizmos::destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Application::Tick(){
	while(lagTime >= TICK_PER_SEC_D_){
		camera->Update();

		earth->setGlobalRotation(3.14f * currentTime, glm::vec3(0, 1, 0));
		earth->Update();

		lagTime -= TICK_PER_SEC_D_;
	}
}

void Application::Draw(){
	for (int i = 0; i < 21; i++){
		Gizmos::addLine(glm::vec3(-10 + i, 0, 10),
						glm::vec3(-10 + i, 0, -10),
						i == 10 ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 0, 0, 1));
		Gizmos::addLine(glm::vec3(10, 0, -10 + i),
						glm::vec3(-10, 0, -10 + i),
						i == 10 ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 0, 0, 1));
	}
	//Setup Planet Draw
	earth->Draw();
	

	//Camera Draw
	camera->UpdateProjectionViewTransform();
	Gizmos::draw(camera->camera_view_transform1());
}

bool Application::Update(){
	if (glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		return false;
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
	currentTime = glfwGetTime();
	elapsedTime = currentTime - previousTime;
	previousTime = currentTime;
	lagTime += elapsedTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1));
	return true;
	
}