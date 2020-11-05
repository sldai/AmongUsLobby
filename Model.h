#pragma once
#include "Object.h"
#include <vector>
#include <string>
#include "utils.h"
#include <algorithm>
#include <numeric>
#include <shader.h>
using namespace std;


class Model : public Object
{
public:
	vector<glm::vec3> vertices; // vertex
	vector<glm::vec3> colors;  // color
	vector<glm::vec3> vertices_normal; // vertex normal
	vector<glm::ivec3> v_indices, vn_indices; // triangle indices
	glm::vec3 ambient, diffuse, specular;
	float shininess;
private:
	GLuint VAO, VBO_v, VBO_vn, VBO_c, EBO;

public:
	Model(std::string objFilename);
	~Model();

	void set_material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	void rotate(float rad, glm::vec3 axis);
	void translate(glm::vec3 p);
	void reset_model();
	void draw(const glm::mat4& view, const glm::mat4& projection, Shader shader);
	void draw_normal(const glm::mat4& view, const glm::mat4& projection, Shader shader);
};

