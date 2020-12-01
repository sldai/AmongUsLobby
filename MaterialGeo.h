#pragma once
#include "Geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "utils.h"
using namespace std;

// 3d model with specified material
class MaterialGeo : public Geometry
{
public:
	glm::vec3 ambient, diffuse, specular;
	float shininess;

public:
	MaterialGeo(string const& path, bool norm=true): Geometry(path)
	{
		vector<glm::vec3> points;
		// extract vertices from the mesh
		auto vertices = geo_model->meshes[0].vertices;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			points.push_back(vertices[i].Position);
		}

		// normalize the model size within [-1,1]
		// use update to furthre adjust the model size
		if (norm)
		{
			model = normalize_matrix(points) * model;
		}
		
	};
	
	
    // render the mesh
    void draw(glm::mat4 T)
	{
		shader_ptr->use();
		shader_ptr->setMat4("model", T * model);
		// material properties
		shader_ptr->setVec3("material.ambient", ambient);
		shader_ptr->setVec3("material.diffuse", diffuse);
		shader_ptr->setVec3("material.specular", specular); // specular lighting doesn't have full effect on this object's material
		shader_ptr->setFloat("material.shininess", shininess);

		// draw mesh
		glBindVertexArray(geo_model->meshes[0].VAO);
		glDrawElements(GL_TRIANGLES, geo_model->meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void set_material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
	}
};

