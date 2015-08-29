#pragma once

#include <aie\Gizmos.h>
#include <glm\glm.hpp>

class Planet
{
public:
	//Constructors
	Planet();
	~Planet();
	
	Planet(float a_radius, int a_rows, int a_cols, glm::vec4 a_color);
	Planet(float a_radius, int a_rows, int a_cols, glm::vec4 a_color, glm::mat4 a_transform);

	//Vars
	float radius;
	int rows, cols;
	glm::vec4 color;
	glm::mat4 transform;
	glm::vec3 center;

	//
	void Update();
	void Draw();

	//Set Values
	void setRadius(float a_radius);
	void setRows(int a_rows);
	void setCols(int a_cols);
	void setColor(glm::vec4 a_color);
	void setTransform(glm::mat4& a_transform);
	void setCenter(glm::vec3 a_center);

};