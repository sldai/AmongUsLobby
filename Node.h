#pragma once
#include <glm/glm.hpp>

class Node {
public:
	Node* parent = nullptr;
	virtual void draw(glm::mat4 C) = 0;
	virtual void update(glm::mat4 C) = 0;
};