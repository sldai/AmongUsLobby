#pragma once
#include "Geometry.h"

class DirLight : public Geometry
{
public:
    glm::vec3 direction;
    glm::vec3 color;
    float brightness;

    bool status;

    DirLight() : status(true) {
        direction = glm::vec3(0.0, -1.0, 0.0);
        color = glm::vec3(1.0);
    }

    DirLight(glm::vec3 direction, glm::vec3 color) : status(true) {
        this->direction = direction;
        this->color = color;
    }

    void draw(glm::mat4 T) {} // do noting
};