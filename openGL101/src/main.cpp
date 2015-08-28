#include <iostream>
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include <aie\Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//Sphere Variables
glm::vec3 planetCenter = { 1,1,1 };
float planetRadius = 2.f;
int planetRows = 20;
int planetCols = 20;
const glm::vec4& planetColor = { .5f, 0, .8f, 1 };
glm::mat4 planetTransform;

int main() {
	if (glfwInit() == false)
		return -1;

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	//What does this do?
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	Gizmos::create();

	glm::mat4 view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	//deltaTime
	float current = 0;
	float last = 0;
	float dt = 0;

	//Set Clear Screen color
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	//
	while (!glfwWindowShouldClose(window)) {
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		current = glfwGetTime();
		dt = current - last;
		last = current;

		std::cout << planetCenter.x << " " << planetCenter.y << " " << planetCenter.z << std::endl;

		Gizmos::clear();

		Gizmos::addTransform(glm::mat4(1));

		glm::vec4 white(1);
		glm::vec4 black(0, 0, 0, 1);

		for (int i = 0; i < 21; i++) {
			Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), i == 10 ? white : black);

			Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), i == 10 ? white : black);
			
		}

		//Draw Sphere
		Gizmos::addSphere(planetCenter, planetRadius, planetRows, planetCols, planetColor, &planetTransform);

		//
		planetTransform = glm::translate(glm::vec3(0, 1, 0))
			//Planet Props
			* glm::rotate(current * 1.14f, glm::vec3(0, 1, .2f));
			//* glm::translate(glm::vec3(0, 0.f, 2.f));

		//
		Gizmos::draw(projection * view);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}