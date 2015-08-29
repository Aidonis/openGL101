#include "Planet.h"

Planet::Planet()
{
	radius = 0;
	rows = 0;
	cols = 0;
	color = glm::vec4(1, 1, 1, 0);
	transform = glm::mat4(1);
	center = glm::vec3(0, 0, 0);

}

Planet::~Planet()
{
}

Planet::Planet(float a_radius, int a_rows, int a_cols, glm::vec4 a_color)
{
	radius = a_radius;
	rows = a_rows;
	cols = a_cols;
	color = a_color;
	center = glm::vec3(0, 0, 0);
	transform = glm::mat4(1);

}

Planet::Planet(float a_radius, int a_rows, int a_cols, glm::vec4 a_color, glm::mat4 a_transform)
{
	radius = a_radius;
	rows = a_rows;
	cols = a_cols;
	color = a_color;
	center = glm::vec3(0, 0, 0);
	transform = a_transform;
}

void Planet::Update()
{

}

void Planet::Draw()
{
	if(&transform == nullptr)
	{
		Gizmos::addSphere(center, radius, rows, cols, color);
	}
	else
	{
		Gizmos::addSphere(center, radius, rows, cols, color, &transform);
	}
	
}

void Planet::setRadius(float a_radius)
{
	radius = a_radius;
}

void Planet::setRows(int a_rows)
{
	rows = a_rows;
}

void Planet::setCols(int a_cols)
{
	cols = a_cols;
}

void Planet::setColor(glm::vec4 a_color)
{
	color = a_color;
}

void Planet::setTransform(glm::mat4& a_transform)
{
	transform = a_transform;
}

void Planet::setCenter(glm::vec3 a_center)
{
	center = a_center;
}