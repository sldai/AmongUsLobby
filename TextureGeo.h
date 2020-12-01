#pragma once
#include <glad/glad.h>
#include "Geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class TextureGeo : public Geometry
{
public:

public:
	TextureGeo(string const& path);
	~TextureGeo();
	void draw(glm::mat4 T)
	{
		shader_ptr->use();
		shader_ptr->setMat4("model", T * model);

		geo_model->Draw(*shader_ptr);
	}
private:

};

TextureGeo::TextureGeo(string const& path) : Geometry(path)
{

}

TextureGeo::~TextureGeo()
{
}