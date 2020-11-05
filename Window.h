#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"
#include <iostream>

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	static Scene* scene;

	// drag
	static bool drag;
	static glm::vec2 mouse_pos;

	// render mode
	static bool render_normal; // whether use the vertex normal as color
	// interaction mode
	static int interaction_mode;
	// Constructors and Destructors
	static void load_scene();
	static void cleanUp();

	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void displayCallback(GLFWwindow* window);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int modes);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif
