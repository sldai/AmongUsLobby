#pragma once
#include "Object.h"

class PointLight
{
public:
	glm::vec3 position;
	glm::vec3 color;

    float constant;
    float linear;
    float quadratic;

    bool status;

    PointLight(): status(true) {
        position = glm::vec3(0.0, 3.0, 0.0);
        color = glm::vec3(1.0);
        constant = 1.0;
        linear = 0.0;
        quadratic = 0.0;
    }

    PointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic): status(true) {
        this->position = position;
        this->color = color;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;
    }
};

