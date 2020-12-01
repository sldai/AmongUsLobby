#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

struct BoundingSphere
{
	glm::vec3 center;
	float R;
	float h;
};

struct BoundingPlane
{
	glm::vec3 o; // origin coordinate 
	glm::vec3 n; // normal
	BoundingPlane()
	{}
	BoundingPlane(glm::vec3 origin, glm::vec3 normal):
		o(origin), n(normal)
	{}
};

bool BS_inter_BS(BoundingSphere a, BoundingSphere b)
{
	float dis = glm::length(a.center - b.center);
	if (dis > (a.R + b.R))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool BS_inter_BP(BoundingSphere a, BoundingPlane b)
{
	glm::vec3 normal = glm::normalize(b.n);
	float dis = glm::dot((a.center - b.o), normal);
	if (abs(dis) > a.R)
	{
		return false;
	}
	else return true;
}
