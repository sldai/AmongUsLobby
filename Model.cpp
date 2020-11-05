#include "Model.h"

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

	auto model = glm::scale(glm::mat4(1.0f), glm::vec3(1 / max_scale, 1 / max_scale, 1 / max_scale)) * glm::translate(glm::mat4(1.0f), glm::vec3(-bias[0], -bias[1], -bias[2]));
	return model;
}


Model::Model(string objFilename)
{
	load_obj(objFilename, vertices, colors, vertices_normal, v_indices, vn_indices);
	// Model matrix. Since the original size of the Model is 2, in order to
	// have a Model of some size, we need to scale the Model by size / 2.
	model = glm::mat4(1.0f);

	// The color of the Model. Try setting it to something else!
	set_material(glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0);



	// Generate a vertex array (VAO) and vertex buffer object (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_v); // vertices
	glGenBuffers(1, &VBO_vn); // vertex normals
	glGenBuffers(1, &VBO_c); // vertex colors
	glGenBuffers(1, &EBO); // vertex indices

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// bind vertex data at VAO 0 pointer
	glBindBuffer(GL_ARRAY_BUFFER, VBO_v);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// bind vertex normal data at VAO 1 pointer
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vn);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices_normal.size(), vertices_normal.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//// bind vertex color data at VAO 2 pointer
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_c);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * v_indices.size(), v_indices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Model::~Model()
{
	// Delete the VBO/EBO and the VAO.
	glDeleteBuffers(1, &VBO_v);
	glDeleteBuffers(1, &VBO_vn);
	glDeleteBuffers(1, &VBO_c);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Model::set_material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

void Model::draw(const glm::mat4& view, const glm::mat4& projection, Shader shader)
{
	// Actiavte the shader program 
	shader.use();

	// Get the shader variable locations and send the uniform data to the shader 
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setMat4("model", model);

	// material properties
	shader.setVec3("material.ambient", ambient);
	shader.setVec3("material.diffuse", diffuse);
	shader.setVec3("material.specular", specular); // specular lighting doesn't have full effect on this object's material
	shader.setFloat("material.shininess", shininess);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, v_indices.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	//glUseProgram(0);
}

void Model::draw_normal(const glm::mat4& view, const glm::mat4& projection, Shader shader)
{
	// Actiavte the shader program 
	shader.use();

	// Get the shader variable locations and send the uniform data to the shader 
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setMat4("model", model);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, v_indices.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	//glUseProgram(0);
}

void Model::rotate(float rad, glm::vec3 axis) {
	model = glm::rotate(rad, axis) * model;
}

void Model::translate(glm::vec3 p) {
	model = glm::translate(p) * model;
}

void Model::reset_model() {
	model = glm::mat4(1.0f) * normalize_matrix(vertices);
}