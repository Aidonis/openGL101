#pragma once
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include <aie\Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "../Planets/Planet.h"
#include "../Camera/Camera.h"
#include "../Camera/FlyCamera.h"

#include <TinyBuild/tiny_obj_loader.h>


#include <STB/stb_image.h>



struct GLFWwindow;



enum ApplicationFail{
	NONE,
	GLFW_INIT,
	GLFW_CREATE_WINDOW,
	OGL_LOAD_FUNCTIONS,
	LOAD_MODEL,
	LOAD_TEXTURE
};

struct Vertex {
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 UV;
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

	//Buffersa
	unsigned int VAO, VBO, IBO;
	unsigned int programID;

	unsigned int textureID;


public:
	Application();
	Application(std::string set_name);
	Application(std::string set_name, int set_width, int set_height);
	~Application();

	ApplicationFail Init();
	void Shutdown();

	bool Update();
	void GenerateGrid(unsigned rows, unsigned cols);
	void Tick();
	void Draw();

	virtual void onInit(){ };

	virtual void onTerm(){ };

	virtual void onStep(){ };

	virtual void onDraw(){ };

	//OBJ Loader
	struct GLInfo {
		unsigned int VAO, VBO, IBO, indexCount;
	};

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<GLInfo> glInfo;

	void createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes);

	struct Image {
		int width, height, format;
		unsigned char* data;
	} ImageInfo;

};