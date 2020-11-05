#pragma once
#include "Object.h"
class DirecLight
{
public:
	glm::vec3 direction;
	glm::vec3 color;
	DirecLight() {
		direction = glm::vec3(0.0, -1.0, 0.0);
		color = glm::vec3(1.0, 1.0, 1.0);
	}
	DirecLight(glm::vec3 direction, glm::vec3 color){
		this->direction = direction;
		this->color = color;
	}

};

