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


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

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
	
	glfwTerminate();
	return 0;
}