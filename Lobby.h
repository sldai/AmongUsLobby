#pragma once
#include <glad/glad.h>
#include "Geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>




class Lobby : public Geometry
{
public:
	float height, width, length;
	float box_size;

	float ambient = 0.0, diffuse = 1, specular = 1;
	float shininess = 32;
	float C = 0.3;
public:
	Lobby(string const& path, bool norm = true) : Geometry(path)
	{
		vector<glm::vec3> points;
		// extract vertices from the mesh
		auto vertices = geo_model->meshes[0].vertices;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			points.push_back(vertices[i].Position);
		}

		// get height and width and length
		float min_max[3][2] = {0.0};
		for (int i = 0; i < 3; ++i)
		{
			// find the min and max x, y, z
			min_max[i][0] = (*std::min_element(
				points.begin(), points.end(),
				[&](glm::vec3& a, glm::vec3& b) { return a[i] < b[i]; }
			))[i];

			min_max[i][1] = (*std::max_element(
				points.begin(), points.end(),
				[&](glm::vec3& a, glm::vec3& b) { return a[i] < b[i]; }
			))[i];

			if (i==0)
			{
				length = min_max[i][1] - min_max[i][0];
			}
			else if (i==1)
			{
				height = min_max[i][1] - min_max[i][0];
			}
			if (i==2)
			{
				width = min_max[i][1] - min_max[i][0];
			}
		}

		// normalize the model size within [-1,1]
		// use update to furthre adjust the model size
		if (norm)
		{
			model = normalize_matrix(points) * model;
		}

		
	}

	// get the normalization matrix for a bunch of points
	glm::mat4 normalize_matrix(std::vector<glm::vec3> points) {
		float min_max[3][2] = { 0.0 };
		vector<float> bias(3);
		float max_scale;

		// centering
		for (int i = 0; i < 3; ++i)
		{
			// find the min and max x, y, z
			min_max[i][0] = (*std::min_element(
				points.begin(), points.end(),
				[&](glm::vec3& a, glm::vec3& b) { return a[i] < b[i]; }
			))[i];

			min_max[i][1] = (*std::max_element(
				points.begin(), points.end(),
				[&](glm::vec3& a, glm::vec3& b) { return a[i] < b[i]; }
			))[i];

			bias[i] = (min_max[i][0] + min_max[i][1]) / 2.0;
		}



		// scaling
		auto norm = [&](glm::vec3& a)
		{
			return sqrt(pow(a[0] - bias[0], 2) + pow(a[1] - bias[1], 2) + pow(a[2] - bias[2], 2));
		};
		max_scale = norm(*std::max_element(
			points.begin(), points.end(),
			[&](glm::vec3& a, glm::vec3& b)
			{ return norm(a) < norm(b); }
		));

		auto model = glm::scale(glm::mat4(1.0f), glm::vec3(1 / max_scale, 1 / max_scale, 1 / max_scale)) * glm::translate(glm::mat4(1.0f), glm::vec3(-bias[0], -min_max[1][0], -bias[2]));
		return model;
	}

	~Lobby();
	void draw(glm::mat4 T)
	{
		shader_ptr->use();
		shader_ptr->setMat4("model", T * model);
		// material properties
		shader_ptr->setFloat("material.ambient", ambient);
		shader_ptr->setFloat("material.diffuse", diffuse);
		shader_ptr->setFloat("material.specular", specular); // specular lighting doesn't have full effect on this object's material
		shader_ptr->setFloat("material.shininess", shininess);
		shader_ptr->setFloat("C", C);
		geo_model->Draw(*shader_ptr);
	}

	float get_height()
	{
		glm::vec4 h_vec(0, height, 0, 0);
		return glm::length(glm::vec3(model* h_vec));
	}

	float get_width()
	{
		glm::vec4 w_vec(0, 0, width, 0);
		return glm::length(glm::vec3(model * w_vec));
	}

	float get_length()
	{
		glm::vec4 l_vec(length, 0, 0, 0);
		return glm::length(glm::vec3(model * l_vec));
	}

	// scaled to specified height
	void scale2height(float tar_h)
	{
		float scale = tar_h / get_height();
		model = glm::scale(model, glm::vec3(scale));
	}


private:

};
