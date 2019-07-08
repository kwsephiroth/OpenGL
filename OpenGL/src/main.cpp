#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <SOIL2/soil2.h>
#include "../Shader.h"
#include "../ModelLoader.h"
#include "../Renderer.h"
#include <iostream>

#define numVAOs 1
#define numVBOs 1
#define numIBOs 1

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

unsigned int vao[numVAOs];
unsigned int vbo[numVBOs];
unsigned int ibo[numIBOs];

unsigned int renderingProgram;

int width, height;
float aspect;
glm::mat4 pMat;

unsigned int positionAttribLocation;
unsigned int textureAttribLocation;

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function 
			<< " file : " << line << std::endl;
		return false;
	}
	return true;
}

static void init(GLFWwindow* window)
{
	Shader shader("res/shaders/vertexShader.glsl", "res/shaders/fragmentShader.glsl");
	renderingProgram = shader.Program;
	positionAttribLocation = glGetAttribLocation(renderingProgram, "position");
	textureAttribLocation = glGetAttribLocation(renderingProgram,  "tex_coord");
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

static void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

static void setup_vertices()
{
	float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f, 0.5f,
		-0.5f, 0.5f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glGenVertexArrays(numVAOs, vao));
	GLCall(glBindVertexArray(vao[0]));

	GLCall(glGenBuffers(numVBOs, vbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo[0]));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));
	
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
	GLCall(glEnableVertexAttribArray(0));

	GLCall(glGenBuffers(numIBOs, ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));//Unbinds any buffer object previously bound (Probably not necessary here)
	GLCall(glBindVertexArray(0)); //Break the existing vertex array object binding after use (Probably not necessary here)
}

static void draw(GLFWwindow* window, double currentTime)
{
	GLCall(glUseProgram(renderingProgram));

	GLCall(glBindVertexArray(vao[0]));
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	//glEnableVertexAttribArray(0);
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]));
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));//Unbinds any buffer object previously bound (Probably not necessary here)
	GLCall(glBindVertexArray(0)); //Break the existing vertex array object binding after use (Probably not necessary here)
	GLCall(glUseProgram(0));
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);//Set vsync

	//Must have a valid OpenGL context before initializing glew
	auto errorCode = glewInit();
	if (errorCode != GLEW_OK)
		std::cout << "glewInit() failed with error code (" << errorCode << ")" << std::endl;


	glfwSetWindowSizeCallback(window, window_size_callback);
	//std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	
	//setup_vertices();
	init(window);

	Renderer r(renderingProgram, positionAttribLocation, textureAttribLocation);
	{
		//Attempt to generate model object
		auto model = ModelLoader::LoadModelFromOBJFile("shuttle", "res/models/shuttle.obj", "res/textures/spstob_1.jpg");
		if (model)
		{
			r.AddModel(std::move(model));
		}
	}

    auto toRadians = [](float degrees) {return (degrees * 2.0f * 3.14159f) / 360.0f; };
	
	glm::mat4 tempModelMatrix;
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		//draw(window, glfwGetTime());
		

		//TODO: Apply any transforms to a model here
		//r.GetModelMatrix("shuttle", tempModelMatrix);
		tempModelMatrix = glm::translate(glm::mat4(1.0f), r.GetModelInitialWorldPosition("shuttle"));
		tempModelMatrix = glm::rotate(tempModelMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		tempModelMatrix = glm::rotate(tempModelMatrix, toRadians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tempModelMatrix = glm::rotate(tempModelMatrix, toRadians(35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		r.SetModelMatrix("shuttle", std::move(tempModelMatrix));

		r.SetAspectRatio(aspect);
		//Now render model(s)
		//r.RenderModels();
		r.RenderModel("shuttle");

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	
	//Clean up
	//glDeleteVertexArrays(numVAOs, vao);
	//glDeleteBuffers(numVBOs, vbo);
	//glDeleteBuffers(numIBOs, ibo);

	glfwTerminate();
	return 0;
}