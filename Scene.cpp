#include "Scene.h"

Scene::Scene() :
	shader_lighting("shaders/lighting.vs", "shaders/lighting.fs"),
	shader_light_cube("shaders/light_cube.vs", "shaders/light_cube.fs"),
	shader_normal("shaders/normal.vs", "shaders/normal.fs"),
	bunny("resources/bunny.obj"),
	sandal("resources/SandalF20.obj"),
	bear("resources/bear.obj"),
	pointlight_bulb("resources/sphere.obj"),
	direclight(glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0)),
	pointlight(glm::vec3(0.0, 1.0, 0.0), glm::vec3(1,0.829,0.829), 1.0f, 0.09f, 0.0f),
	spotlight(glm::vec3(0.0, 0.0, 2.0),
		glm::vec3(0.0,0.0,-1.0),
		glm::cos(glm::radians(12.5f)),
		glm::vec3(0.75164,0.60648,0.22648),
		1.0f,
		0.09f, 
		0.0f)
{
	currObj=&bunny;
	bunny.reset_model();
	sandal.reset_model();
	bear.reset_model();
	bunny.set_material(glm::vec3(0.0215, 0.1745, 0.0215), glm::vec3(0.0), glm::vec3(0.633,0.727811,0.633), 32.0); // shiny, high specular, no diffuse
	sandal.set_material(glm::vec3(0.135,0.2225,0.1575), glm::vec3(0.54,0.89,0.63), glm::vec3(0.0), 32.0); // no specular, only diffuse
	bear.set_material(glm::vec3(0.25,0.20725,0.20725), glm::vec3(1,0.829,0.829), glm::vec3(0.296648,0.296648,0.296648), 32.0); // significant diffuse and specular
	pointlight_bulb.set_material(glm::vec3(1.0), glm::vec3(0.0), glm::vec3(0.0), 32.0);
	pointlight_bulb.reset_model();
	pointlight_bulb.setModel(glm::scale(glm::vec3(0.1))*pointlight_bulb.getModel());
	pointlight_bulb.translate(pointlight.position);

}

Scene::~Scene() {
	// Delete the shader program.
	;
}

void Scene::draw(const glm::mat4& view, const glm::mat4& projection) {
	shader_lighting.use();
	shader_lighting.setVec3("pointlight.position", pointlight.position);
	shader_lighting.setVec3("pointlight.color", pointlight.color*float(pointlight.status));
	shader_lighting.setFloat("pointlight.constant", pointlight.constant);
	shader_lighting.setFloat("pointlight.linear", pointlight.linear);
	shader_lighting.setFloat("pointlight.quadratic", pointlight.quadratic);

	shader_lighting.setVec3("spotlight.position", spotlight.position);
	shader_lighting.setVec3("spotlight.color", spotlight.color * float(spotlight.status));
	shader_lighting.setFloat("spotlight.cutOff", spotlight.cutOff);
	shader_lighting.setVec3("spotlight.direction", spotlight.direction);
	shader_lighting.setFloat("spotlight.constant", spotlight.constant);
	shader_lighting.setFloat("spotlight.linear", spotlight.linear);
	shader_lighting.setFloat("spotlight.quadratic", spotlight.quadratic);
	auto viewPos = glm::vec3(glm::inverse(view)[3]);

	shader_lighting.setVec3("viewPos", viewPos);
	currObj->draw(view, projection, shader_lighting);

	shader_lighting.setVec3("spotlight.color", glm::vec3(0.0));
	pointlight_bulb.draw(view, projection, shader_lighting);

}


void Scene::draw_normal(const glm::mat4& view, const glm::mat4& projection) {
	currObj->draw(view, projection, shader_normal);
}

void Scene::set_currObj(string s) {
	if (s=="bunny")
	{
		currObj = &bunny;
	}
	else if (s=="sandal")
	{
		currObj = &sandal;
	}
	else if (s=="bear")
	{
		currObj = &bear;
	}
}


void Scene::rotate_spotlight(float angle, glm::vec3 axis) {
	spotlight.position = glm::vec3(glm::rotate(angle, axis) * glm::vec4(spotlight.position, 1));
	spotlight.direction = -spotlight.position/glm::length(spotlight.position);
}