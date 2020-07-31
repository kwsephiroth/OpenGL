#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <SOIL2/soil2.h>
#include "../Shader.h"
#include "../ModelLoader.h"
#include "../MaterialLoader.h"
#include "../Renderer.h"
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <stdlib.h>
#include <thread>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

unsigned int renderingProgram;

int width, height;
float aspect;
glm::mat4 pMat;
bool keys[1024];

unsigned int positionAttribLocation;
unsigned int textureAttribLocation;

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

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

static void print_mat4(const glm::mat4& m)
{
	for (size_t j = 0; j < 4; ++j)
	{
		std::cout << m[0][j] << " " << m[1][j] << " " << m[2][j] << " " << m[3][j] << std::endl;
	}
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

	//Set up callback functions to capture and respond to events
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	//std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	
	//setup_vertices();
	init(window);

	Renderer renderer(renderingProgram, positionAttribLocation, textureAttribLocation);//A single VAO is generated in this constructor
	{
		//Attempt to generate model object
		//auto model = ModelLoader::LoadModelFromOBJFile("shuttle", "res/models/shuttle.obj", "res/textures/spstob_1.jpg");
		auto batMtlMapPtr = MaterialLoader::LoadMaterialFromMtlFile("res/models/bat.mtl");
		auto batModelPtr = ModelLoader::LoadModelFromOBJFile("bat", "res/models/bat.obj", "");
		auto mangoTreeMtlMapPtr = MaterialLoader::LoadMaterialFromMtlFile("res/models/mango_tree.mtl");
		auto mangoTreeModelPtr = ModelLoader::LoadModelFromOBJFile("mango_tree", "res/models/mango_tree.obj", "");

		if (!batModelPtr || !mangoTreeModelPtr)
		{
			return -1;
		}
		
		renderer.AddModel(std::move(batModelPtr));
		renderer.AddModel(std::move(mangoTreeModelPtr));
		renderer.CopyFromMaterialsMap(std::move(batMtlMapPtr));
		renderer.CopyFromMaterialsMap(std::move(mangoTreeMtlMapPtr));
	}

    auto toRadians = [](float degrees) {return (degrees * 2.0f * 3.14159f) / 360.0f; };
	
	glm::mat4 tempModelMatrix = glm::mat4(1.0f);//Initialize to identity matrix

	float yRotAngle = 0.0f;
	float xRotAngle = 0.0f;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	float speed = 120.0f;//frames per second
	int fps = 0;
	/* initialize random seed: */
	//srand(time(NULL));
	int random_number = (rand() % 100);
	const int GROUND_LEVEL = -8.0f;

	const int TREE_COUNT = 10000;
	std::vector<int> tree_rands;
	for (size_t i = 0; i < TREE_COUNT; ++i)
	{
		tree_rands.push_back((rand() % 100));
	}

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{	

		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		/* Render here */
		glClear(GL_DEPTH_BUFFER_BIT);

		float r = 135.0f / 255.0f;
		float g = 206.0f / 255.0f;
		float b = 235.0f / 255.0f;
		float a = 1.0f;
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		//draw(window, glfwGetTime());

		if (keys[GLFW_KEY_S]) //UP Movement
		{
			xRotAngle += (deltaTime * speed);
		}

		if (keys[GLFW_KEY_W])//DOWN/Crouch Movement
		{
			xRotAngle -= (deltaTime * speed);
		}

		if (keys[GLFW_KEY_D])//LEFT movement
		{
			yRotAngle += (deltaTime * speed);
		}

		if (keys[GLFW_KEY_A])//RIGHT movement
		{
			yRotAngle -= (deltaTime * speed);
		}

		//bat 1
		glm::vec3 euler_angles(glm::radians(xRotAngle), glm::radians(yRotAngle), 0);
		//glm::vec3 euler_angles(0, (float)glfwGetTime() * speed, 0);
		glm::quat my_quat = glm::quat(euler_angles);
		//glm::quat my_quat2 = glm::angleAxis(glm::radians(xRotAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		//tempModelMatrix = glm::translate(tempModelMatrix, glm::vec3(-0.8f, -0.8f, 0.0f));	
		tempModelMatrix = glm::toMat4(my_quat);
		tempModelMatrix = glm::scale(tempModelMatrix, glm::vec3(.3f, .3f, .3f));
		//tempModelMatrix = glm::rotate(tempModelMatrix, toRadians(xRotAngle), glm::vec3(1.0f, 0.0f, 0.0f));		
		//tempModelMatrix = glm::rotate(tempModelMatrix, toRadians(yRotAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	
		renderer.SetModelMatrix("bat", std::move(tempModelMatrix));
		renderer.RenderModel("bat");

		//bat 2
		//tempModelMatrix = glm::mat4(1.0f);//Initialize to identity matrix
		////tempModelMatrix = glm::translate(tempModelMatrix, glm::vec3(0.8f, -0.8f, 0.0f));
		//tempModelMatrix = glm::translate(tempModelMatrix, glm::vec3(-0.3f, 0.3f, 0.0f));
		//tempModelMatrix = glm::scale(tempModelMatrix, glm::vec3(.3f, .3f, .3f));
		//tempModelMatrix = glm::rotate(tempModelMatrix, toRadians(xRotAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		//tempModelMatrix = glm::rotate(tempModelMatrix, toRadians(yRotAngle), glm::vec3(0.0f, 1.0f, 0.0f));

		//renderer.SetModelMatrix("bat", std::move(tempModelMatrix));
		//renderer.RenderModel("bat");


		//Mango tree
		for (size_t i = 0; i < TREE_COUNT; ++i)
		{
			auto tf = tree_rands[i] + i;
			tempModelMatrix = glm::mat4(1.0f);//Initialize to identity matrix
			tempModelMatrix = glm::scale(tempModelMatrix, glm::vec3(.09f, .09f, .09f));
			tempModelMatrix = glm::translate(tempModelMatrix, glm::vec3(cos(tf) * tree_rands[i] * i/*15.0f*/, GROUND_LEVEL, sin(tf) * tree_rands[i] * i));//-5.0f));
			tempModelMatrix = glm::rotate(tempModelMatrix, toRadians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			renderer.SetModelMatrix("mango_tree", std::move(tempModelMatrix));
			renderer.RenderModel("mango_tree");
		}

		renderer.SetAspectRatio(aspect);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		lastTime = currentTime;

		/* Poll for and process events */
		glfwPollEvents();

		//reset temp matrix to identity matrix per iteration
		tempModelMatrix = glm::mat4(1.0f);

	}
	tree_rands.clear();
	glfwTerminate();
	return 0;
}
