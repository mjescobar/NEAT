#ifndef OPENGL_INITIALIZATION_HPP
#define OPENGL_INITIALIZATION_HPP
	
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class OpenGLInitialization
{

public:
	void initialization();
	void cursor_enter_callback(GLFWwindow* window, int entered);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	GLFWwindow * window;
	GLFWcursor * cursor;
	glm::mat4 Projection;
	glm::mat4 View;
	float cameraDistance;
	void setDistanceCamera(float distance);
	float getDistanceCamera(); 

};

#endif