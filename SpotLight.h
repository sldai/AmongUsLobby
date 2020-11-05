#pragma once
#include "Object.h"

class SpotLight
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff; // cos(cutoff angle)
    
    glm::vec3 color;
    float constant;
    float linear;
    float quadratic;

    bool status; // turn on or off

    SpotLight() {
        position = glm::vec3(0.0, 0.0, 2.0);
        direction = glm::vec3(0.0, 0.0, -1.0);
        cutOff = glm::cos(glm::radians(12.5f));
        color = glm::vec3(1.0);
        constant = 1.0;
        linear = 0.0;
        quadratic = 0.0;
        status = true;
    }

    SpotLight(glm::vec3 position,
    glm::vec3 direction,
    float cutOff,
    glm::vec3 color,
    float constant,
    float linear,
    float quadratic) {
        this->position = position;
        this->direction = direction;
        this->cutOff = cutOff;
        this->color = color;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;
        status = true;
    }
};

