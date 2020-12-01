#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"
#include "camera.h"
#include <iostream>

class Window
{
public:
	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Camera Matrices
	static Camera* camera_ptr;

	static Scene* scene_ptr;

	// time management
	static float last_t;  
	static float delta_t;

	// cursor management
	static bool firstMouse;
	static float lastX, lastY;

	static bool view_mode;

	// Constructors and Destructors
	static void create_scene();
	static void cleanUp();

	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback(GLFWwindow* window);
	static void displayCallback(GLFWwindow* window);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void mouse_button_callback(GLFWwindow* window, int button, int action, int modes);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	//static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Project 4";


Scene* Window::scene_ptr = nullptr;
Camera* Window::camera_ptr = nullptr;

float Window::last_t = glfwGetTime();
float Window::delta_t = 0.0;

bool Window::firstMouse = true;
float Window::lastX;
float Window::lastY;

bool Window::view_mode = true;

GLFWwindow* Window::createWindow(int width, int height)
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);


	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}


void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);
}

void Window::create_scene() {
	scene_ptr = new Scene();
	camera_ptr = new Camera(glm::vec3(0,29.6323,22.2906
), glm::vec3(0, 0.544641, - 0.838668), -90, -57);

	//camera_ptr = new Camera(glm::vec3(0, 10, 0
	//));
}

void Window::cleanUp()
{
	delete scene_ptr;
	delete camera_ptr;
}

void Window::idleCallback(GLFWwindow* window)
{
	float cur_t = glfwGetTime();
	delta_t = cur_t - last_t;
	last_t = cur_t;

	scene_ptr->update(delta_t);
}

void Window::displayCallback(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera_ptr->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera_ptr->Zoom), (float)width / (float)height, 0.1f, 100.0f);
	scene_ptr->draw(view, projection, camera_ptr->Position);
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	//	camera_ptr->ProcessKeyboard(FORWARD, delta_t);
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	//	camera_ptr->ProcessKeyboard(BACKWARD, delta_t);
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//	camera_ptr->ProcessKeyboard(LEFT, delta_t);
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//	camera_ptr->ProcessKeyboard(RIGHT, delta_t);
	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	//	camera_ptr->ProcessKeyboard(UP, delta_t);
	//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	//	camera_ptr->ProcessKeyboard(DOWN, delta_t);
	float v = 2;
	auto& player = scene_ptr->players[0];
	

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{	
		if (!player->run_flag)
		{
			player->start();
			scene_ptr->foot_sound.reset();
		}
		player->yaw = glm::radians(180.0f);
		player->v = v;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (!player->run_flag)
		{
			player->start();
			scene_ptr->foot_sound.reset();
		}
		player->yaw = glm::radians(0.0f);
		player->v = v;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (!player->run_flag)
		{
			player->start();
			scene_ptr->foot_sound.reset();
		}
		player->yaw = glm::radians(90.0f);
		player->v = v;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (!player->run_flag)
		{
			player->start();
			scene_ptr->foot_sound.reset();
		}
		player->yaw = glm::radians(-90.0f);
		player->v = v;
	}
	else { 
		if (player->run_flag)
		{
			player->stop();
			scene_ptr->foot_sound.reset();
		}
	}


	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_V:
			cout << scene_ptr->players[0]->x << " " << scene_ptr->players[0]->z << endl;
			break;
		case GLFW_KEY_R:
			scene_ptr->players[0]->astro_ptr->switch_model();
			break;
		default:
			break;
		}
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera_ptr->ProcessMouseMovement(xoffset, yoffset);
}

#endif
