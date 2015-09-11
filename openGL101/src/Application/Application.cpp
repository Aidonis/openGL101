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

	//Load Texture File
	ImageInfo.width = 0;
	ImageInfo.height = 0;
	ImageInfo.format = 0;
	ImageInfo.data = stbi_load("./resource/textures/crate.png", &ImageInfo.width, &ImageInfo.height, &ImageInfo.format, STBI_default);
	if(ImageInfo.data == nullptr){
		return LOAD_TEXTURE;
	}

	//Bind Texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageInfo.width, ImageInfo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageInfo.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(ImageInfo.data);

	//Load Model File
	std::string err = tinyobj::LoadObj(shapes, materials, "./models/stanford/bunny.obj");
	if(err.length() != 0){
		std::cout << "Error loading OBJ file:\n" << err << std::endl;
		return LOAD_MODEL;
	}

	//Set Clear Screen color
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	previousTime = glfwGetTime();

	earth = new Planet();
	earth->setPosition(glm::vec3(0, 2, 0));
	earth->setColor(glm::vec4(.5f, 0, .8f, 1));

	//Texture Shader
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Color; \
							layout(location=2) in vec2 TexCoord; \
							out vec2 vTexCoord; \
							uniform mat4 ProjectionView; \
							void main(){ \
							vTexCoord = TexCoord; \
							gl_Position = ProjectionView * Position; \
							}";

	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							out vec4 FragColor; \
							uniform sampler2D diffuse; \
							void main() { \
							FragColor = texture(diffuse, vTexCoord);\
							}";

	//Shader Shit
	//const char* vsSource = "#version 410\n \
	//						layout(location=0) in vec4 Position; \
	//						layout(location=1) in vec4 Color; \
	//						out vec4 vColor; \
	//						uniform mat4 ProjectionView; \
	//						void main() {vColor = Color; gl_Position = ProjectionView * Position; }";
	//const char* fsSource = "#version 410\n \
	//						in vec4 vColor; \
	//						out vec4 FragColor; \
	//						void main() { FragColor = vColor; }";

	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if(success == GL_FALSE){
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

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

void Application::createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes){
	glInfo.resize(shapes.size());

	for (unsigned int meshIndex = 0; meshIndex < shapes.size(); meshIndex++){
		glGenVertexArrays(1, &glInfo[meshIndex].VAO);
		glGenBuffers(1, &glInfo[meshIndex].VBO);
		glGenBuffers(1, &glInfo[meshIndex].IBO);
		glBindVertexArray(glInfo[meshIndex].VAO);

		unsigned int float_count = shapes[meshIndex].mesh.positions.size();
		float_count += shapes[meshIndex].mesh.normals.size();
		float_count += shapes[meshIndex].mesh.texcoords.size();

		std::vector<float> vertex_data;
		vertex_data.reserve(float_count);

		vertex_data.insert(vertex_data.end(), shapes[meshIndex].mesh.positions.begin(), shapes[meshIndex].mesh.positions.end());
		vertex_data.insert(vertex_data.end(), shapes[meshIndex].mesh.normals.begin(), shapes[meshIndex].mesh.normals.end());

		glInfo[meshIndex].indexCount = shapes[meshIndex].mesh.indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, glInfo[meshIndex].VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glInfo[meshIndex].IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[meshIndex].mesh.indices.size() * sizeof(unsigned int), shapes[meshIndex].mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*shapes[meshIndex].mesh.positions.size()));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	glUseProgram(programID);

	unsigned int projectionViewUniform = glGetUniformLocation(programID, "ProjectionView");

	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(camera->CameraViewTransform));
	for (unsigned int i = 0; i < glInfo.size(); i++) {
		glBindVertexArray(glInfo[i].VAO);
		glDrawElements(GL_TRIANGLES, glInfo[i].indexCount, GL_UNSIGNED_INT, 0);
	}
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

void Application::GenerateGrid(unsigned int rows, unsigned int cols) {
	Vertex* aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			aoVertices[r * cols + c].position = glm::vec4((float)c, 0, (float)r, 1);
			//aoVertices[r * cols + c].UV = 
			aoVertices[r * cols + c].UV.x = 1-(float)r/(rows - 1);
			aoVertices[r * cols + c].UV.y = (float)c/(cols - 1);
			//create arbitrary color based off something that might not be related to tiling a texture
			//glm::vec3 color = glm::vec3(sinf((c / (float)(cols - 1)) * (r / (float)(rows - 1))));
		}
	}
	
	//aoVertices[0].UV.x = 0;
	//aoVertices[0].UV.y = 0;
	//aoVertices[1].UV.x = 0.f;
	//aoVertices[1].UV.y = 1.f;
	//aoVertices[2].UV.x = 1.f;
	//aoVertices[2].UV.y = 0.f;
	//aoVertices[3].UV.x = 1.f;
	//aoVertices[3].UV.y = 1.f;

	//defining index count based off quad count (2 triangles per quad)
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); r++){
		for (unsigned int c = 0; c < (cols - 1); c++){
			//triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			
			//triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
			
		}
	}


	
	//Generate a VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Generate Vertex
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);

	//Index buffer object
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	//Enable vertex attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

	//
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//openGL Draw Shit
	glUseProgram(programID);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	unsigned int projectionViewUniform = glGetUniformLocation(programID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(camera->CameraViewTransform));

	//set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//tell shader where it is
	projectionViewUniform = glGetUniformLocation(programID, "diffuse");
	glUniform1i(projectionViewUniform, 0);

	//Draw
	glBindVertexArray(VAO);
	unsigned int indexCount = (rows - 1) * (cols - 1) * 6;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	delete[] auiIndices;
	delete[] aoVertices;
}