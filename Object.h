#ifndef _OBJECT_H_
#define _OBJECT_H_
#define apple
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Object
{
protected:
	glm::mat4 model;
	glm::vec3 color;

public:
	glm::mat4 getModel() { return model; }
	void setModel(glm::mat4 model) { this->model = model; }
	glm::vec3 getColor() { return color; }

};

#endif

