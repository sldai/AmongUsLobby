#pragma once
#include "Node.h"
#include <vector>
#include <glm/glm.hpp>

using namespace std;

class Transform : public Node
{
public:
	Transform(glm::mat4 model);
	~Transform();
	void draw(glm::mat4 T);
	void update(glm::mat4 T);
	void addChild(Node* new_node);
	void set_model(glm::mat4 T) { model = T; }

	glm::mat4 model;
	vector<Node*> child;
private:

};

Transform::Transform(glm::mat4 model)
{
	this->model = model;
}

void Transform::draw(glm::mat4 T)
{
	for (auto childnode_ptr : child)
	{
		childnode_ptr->draw(T * model);
	}
}

void Transform::update(glm::mat4 T)
{
	model = T * model;
}

void Transform::addChild(Node* new_node)
{
	child.push_back(new_node);
	new_node->parent = this;
}



Transform::~Transform()
{
	// delete child nodes 
	for (auto elem : child)
	{
		delete elem;
	}
}