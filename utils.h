#pragma once
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <random>
using namespace std;

// cout << vec3
ostream& operator<< (ostream& out, const glm::vec3& vec);

// cout << mat4
ostream& operator<< (ostream& out, const glm::mat4& mat);

// load obj file
vector<glm::vec3> load_obj(string fname);
void load_obj(string fname, vector<glm::vec3>& vertices, // verted
	vector<glm::vec3>& colors,
	vector<glm::vec3>& vertices_normal, // vertex normal
	vector<glm::ivec3>& v_indices, vector<glm::ivec3>& vn_indices);

// generate random point clouds
vector<glm::vec3> generate_point_clouds(int num);