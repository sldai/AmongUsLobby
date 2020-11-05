#include "Window.h"
#include <nanogui/button.h>
// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Project 2";


// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 2);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

Scene* Window::scene;

// drag
bool Window::drag;
glm::vec2 Window::mouse_pos;

// render mode
bool Window::render_normal = false;
// interaction mode
int Window::interaction_mode = 1;

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

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0),
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::load_scene() {
	scene = new Scene();
}

void Window::cleanUp()
{
	delete scene;
}

void Window::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw scene
	if (render_normal)
	{
		scene->draw_normal(view, projection);
	}
	else
	{
		scene->draw(view, projection);
	}
	

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	float pointModifySize = 5.0;
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		case GLFW_KEY_F1:
			scene->set_currObj("bunny");
			break;
		case GLFW_KEY_F2:
			scene->set_currObj("sandal");
			break;
		case GLFW_KEY_F3:
			scene->set_currObj("bear");
			break;
		case GLFW_KEY_R:
			scene->currObj->reset_model();
			break;
		case GLFW_KEY_N:
			render_normal = !render_normal;
			break;
		case GLFW_KEY_1:
			interaction_mode = 1;
			break;
		case GLFW_KEY_2:
			interaction_mode = 2;
			break;
		case GLFW_KEY_3:
			interaction_mode = 3;
			break;
		case GLFW_KEY_4:
			interaction_mode = 4;
			break;
		case GLFW_KEY_S:
			scene->spotlight.status = !scene->spotlight.status;
			break;
		case GLFW_KEY_P:
			scene->pointlight.status = !scene->pointlight.status;
			break;
		default:
			break;
		}
	}
}

glm::vec3 trackBallMapping(glm::vec2 point, glm::vec2 windowSize) {
	glm::vec3 v;
	float d;
	v.x = (2.0 * point.x - windowSize.x) / windowSize.x;
	v.y = (windowSize.y - 2.0 * point.y) / windowSize.y;
	v.z = 0.0;
	d = glm::length(v);
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d * d);
	v = v / glm::length(v);
	return v;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int modes) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		mouse_pos = glm::vec2(xpos, ypos);
		drag = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		drag = false;
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (drag)
	{
		auto windowSize = glm::vec2(width, height);
		auto v = trackBallMapping(mouse_pos, windowSize);
		auto mouse_pos_now = glm::vec2(xpos, ypos);
		auto w = trackBallMapping(mouse_pos_now, windowSize);
		auto axis = glm::cross(v, w - v); // rotation axis
		auto angle = glm::length(axis);
		axis = axis / angle;

		if (interaction_mode == 1)
		{
			scene->currObj->rotate(angle, axis);
		}
		else if (interaction_mode == 2)
		{
			scene->pointlight.position = glm::vec3(glm::rotate(angle, axis) * glm::vec4(scene->pointlight.position, 1));
			scene->pointlight_bulb.rotate(angle, axis);
		}
		else if (interaction_mode == 3)
		{
			scene->currObj->rotate(angle, axis);
			scene->pointlight.position = glm::vec3(glm::rotate(angle, axis) * glm::vec4(scene->pointlight.position, 1));
			scene->pointlight_bulb.rotate(angle, axis);
		}
		else if (interaction_mode == 4)
		{
			scene->rotate_spotlight(angle, axis);
		}

		
		mouse_pos = mouse_pos_now;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	double scale = 0.1;
	double x_off = xoffset * scale;
	double y_off = yoffset * scale;
	if (interaction_mode==1)
	{
		scene->currObj->setModel(glm::scale(glm::vec3(1 + y_off)) * scene->currObj->getModel());
	}
	else if (interaction_mode==2)
	{
		scene->pointlight.position = glm::vec3(glm::scale(glm::vec3(1 + y_off)) * glm::vec4(scene->pointlight.position, 1));
		scene->pointlight_bulb.setModel(glm::scale(glm::vec3(1 + y_off)) * scene->pointlight_bulb.getModel());
	}
	else if (interaction_mode==3)
	{
		scene->currObj->setModel(glm::scale(glm::vec3(1 + y_off)) * scene->currObj->getModel());
		scene->pointlight.position = glm::vec3(glm::scale(glm::vec3(1 + y_off)) * glm::vec4(scene->pointlight.position, 1));
		scene->pointlight_bulb.setModel(glm::scale(glm::vec3(1 + y_off)) * scene->pointlight_bulb.getModel());
	}
	
}


