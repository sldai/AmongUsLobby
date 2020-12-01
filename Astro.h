#pragma once
#include "Geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
using namespace std;


// 3d model with specified material
class Astro : public Geometry
{
public:
	glm::vec3 color;
	float ambient=0, diffuse=1, specular=1;
	float shininess=32;
	float C = 0.3;
	float height, width, length;
	Model* still_mod, * move1_mod, * move2_mod;
	bool visible = true;
public:
	Astro(string const& still_path, string const& move1_path, string const& move2_path, bool norm = true) : Geometry()
	{
		still_mod = new Model(still_path, false);
		move1_mod = new Model(move1_path, false);
		move2_mod = new Model(move2_path, false);
		geo_model = still_mod;

		vector<glm::vec3> points;
		// extract vertices from the mesh
		auto vertices = geo_model->meshes[0].vertices;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			points.push_back(vertices[i].Position);
		}

		// get height and width and length
		float min_max[3][2] = { 0.0 };
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

			if (i == 0)
			{
				length = min_max[i][1] - min_max[i][0];
			}
			else if (i == 1)
			{
				height = min_max[i][1] - min_max[i][0];
			}
			if (i == 2)
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

	};

	~Astro()
	{
		delete still_mod;
		delete move1_mod;
		delete move2_mod;
		still_mod = nullptr;
		move1_mod = nullptr;
		move2_mod = nullptr;
		geo_model = nullptr;
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
	// render the mesh
	void draw(glm::mat4 T)
	{
		if (!visible)
		{
			return;
		}
		shader_ptr->use();
		shader_ptr->setMat4("model", T * model);
		// material properties
		shader_ptr->setVec3("material.color", color);
		shader_ptr->setFloat("material.ambient", ambient);
		shader_ptr->setFloat("material.diffuse", diffuse);
		shader_ptr->setFloat("material.specular", specular); // specular lighting doesn't have full effect on this object's material
		shader_ptr->setFloat("material.shininess", shininess);
		shader_ptr->setFloat("C", C);

		// draw mesh
		glBindVertexArray(geo_model->meshes[0].VAO);
		glDrawElements(GL_TRIANGLES, geo_model->meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	float get_height()
	{
		glm::vec4 h_vec(0, height, 0, 0);
		return glm::length(glm::vec3(model * h_vec));
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

	void switch_model()
	{
		if (geo_model == still_mod)
		{
			geo_model = move1_mod;
		}
		else if(geo_model == move1_mod)
		{
			geo_model = move2_mod;
		}
		else if (geo_model == move2_mod)
		{
			geo_model = move1_mod;
		}
	}
};

