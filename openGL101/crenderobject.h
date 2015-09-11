#pragma once
#include <GLFW/glfw3.h>


struct RenderObject
{
	int VBO, IBO, VAO, size;
};

struct Window
{
	int width, height;
	GLFWwindow *window;
};

RenderObject makeGrid(int rows, int columns);
RenderObject loadObj(...);

RenderObject loadFBX(...);
RenderObject makeSomeRandomMesh(...);

void		 freeRenderObject(RenderObject &ro);

// discreet
// discrete*


//void Application::GenerateGrid(unsigned int rows, unsigned int cols) {
//	Vertex* aoVertices = new Vertex[rows * cols];
//	for (unsigned int r = 0; r < rows; r++) {
//		for (unsigned int c = 0; c < cols; c++) {
//			aoVertices[r * cols + c].position = glm::vec4((float)c, 0, (float)r, 1);
//
//			//create arbitrary color based off something that might not be related to tiling a texture
//			glm::vec3 color = glm::vec3(sinf((c / (float)(cols - 1)) * (r / (float)(rows - 1))));
//		}
//	}
//	//More Stuff here soon
//	//defining index count based off quad count (2 triangles per quad)
//	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
//	unsigned int index = 0;
//	for (unsigned int r = 0; r < (rows - 1); r++) {
//		for (unsigned int c = 0; c < (cols - 1); c++) {
//			//triangle 1
//			auiIndices[index++] = r * cols + c;
//			auiIndices[index++] = (r + 1) * cols + c;
//			auiIndices[index++] = (r + 1) * cols + (c + 1);
//
//			//triangle 2
//			auiIndices[index++] = r * cols + c;
//			auiIndices[index++] = (r + 1) * cols + (c + 1);
//			auiIndices[index++] = r * cols + (c + 1);
//
//		}
//	}