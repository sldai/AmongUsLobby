#pragma once
#include <iostream>
#include "Object.h"
#include "Model.h"
#include "DirecLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <shader.h>

class Scene
{
public:
	// Shader Program ID
	Shader shader_lighting, shader_light_cube, shader_normal;
	Model bunny, sandal, bear;
	Model* currObj;
	DirecLight direclight;
	PointLight pointlight;
	Model pointlight_bulb;
	SpotLight spotlight;

	Scene();
	~Scene();
	void draw(const glm::mat4& view, const glm::mat4& projection);
	void draw_normal(const glm::mat4& view, const glm::mat4& projection);
	void set_currObj(string s);
	void rotate_spotlight(float angle, glm::vec3 axis);
};

