#pragma once
#include <glad/glad.h>
#include "Geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "utils.h"

using namespace std;

class EnvMappingGeo : public Geometry
{
public:
	unsigned int cubemapTexture;

public:
	EnvMappingGeo(string const& path, unsigned int textureID) : Geometry(path), cubemapTexture(textureID)
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
		model = normalize_matrix(points) * model;
	};


	// render the mesh
	void draw(glm::mat4 T)
	{
		shader_ptr->use();
		shader_ptr->setMat4("model", T * model);
		shader_ptr->setInt("skybox", GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glBindVertexArray(geo_model->meshes[0].VAO);
		glDrawElements(GL_TRIANGLES, geo_model->meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

};

