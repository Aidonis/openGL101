#ifndef _APP_H_
#define _APP_H_

#include <glm/glm.hpp>

struct GLFWwindow;

class
{
	GLFWwindow *window;
	float deltaTime, totalTime, lastTime;
	glm::mat4 projection, view;

public:
	void init(int w, int h, const char *title);
	void term();
	void step();
	void draw();

	virtual void onInit() {};
	virtual void onTerm() {};
	virtual void onStep() {};
	virtual void onDraw() {};

	float getDeltaTime() { return deltaTime };
	float getTotalTime() { return totalTime };
};



#endif //_APP_H_