#pragma once
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <memory>
#include <string>

class WindowManager
{
private:
	//std::unique_ptr<GLFWwindow> m_window = nullptr;
	glm::vec3 m_cameraPosition;

	std::string m_windowName;
	bool m_initialized = false;
	int m_windowHeight;
	int m_windowWidth;

	void window_size_callback(GLFWwindow* win, int newWidth, int newHeight);
public:
	WindowManager(int windowWidth, int windowHeight, std::shared_ptr<glm::mat4> perspectiveMatrix, const std::string& windowName="");
	~WindowManager();

	void SetOpenGLVersion(unsigned int major, unsigned int minor);
	//bool WindowShouldClose() const { glfwWindowShouldClose(m_window.get()); }
	inline bool WindowInitialized() const { return m_initialized; }
	float GetAspectRatio() const { return ((float)m_windowWidth / (float)m_windowHeight); }
	const glm::vec3 GetCameraPosition() const { return m_cameraPosition; }
};

