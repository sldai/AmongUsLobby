#pragma once
#include "Node.h"
#include <glm/glm.hpp>
#include <learnopengl/model.h>
#include <learnopengl/shader.h>

class Geometry : public Node
{
public:
	Model* geo_model;
	glm::mat4 model; // transformation matrix
	Shader* shader_ptr; // shader pointer

public:
	Geometry(string const& path, glm::mat4 T, bool gamma);
	Geometry();
	~Geometry();
	void update(glm::mat4 C);
	void use_shader(Shader* s) { shader_ptr = s; }
private:

};

Geometry::Geometry()
{
	geo_model = nullptr;
	model = glm::mat4(1.0f);
	shader_ptr = nullptr;
}

Geometry::Geometry(string const& path, glm::mat4 T = glm::mat4(1.0f), bool gamma = false)
{
	geo_model = new Model(path, gamma);
	model = T;
	shader_ptr = nullptr;
}

void Geometry::update(glm::mat4 T)
{
	model = T * model;
}

Geometry::~Geometry()
{
	if (geo_model != nullptr)
	{
		delete geo_model;
	}	
}

