#include "Mesh.h"
#include <string>

Mesh::Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture*> tex)
{
	vertices = vert;
	indices = ind;
	textures = tex;

	setupMesh();
}

void Mesh::Draw(const Shader* shader)
{
	unsigned int diffuseNr = 0;
	unsigned int normalNr = 0;
	unsigned int heightNr = 0;
	unsigned int specularNr = 0;

	shader->Use();

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->GetID());

		std::string number;
		std::string type = textures[i]->GetType();
		if (type == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (type == "texture_specular")
			number = std::to_string(specularNr++);
		else if (type == "texture_normal")
			number = std::to_string(normalNr++);
		else if (type == "texture_height")
			number = std::to_string(heightNr++);

		shader->SetInt(type + number, i);
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	for (unsigned int i = 0; i < textures.size(); i++)
	{

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

}