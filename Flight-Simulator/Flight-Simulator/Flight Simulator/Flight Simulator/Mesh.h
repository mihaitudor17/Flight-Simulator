#pragma once
#include "Texture.h"
#include "Shader.h";
#include "VertexStruct.h"
#include <vector>

class Mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	//void Draw_by_Renderer(Shader& shader, Renderer& renderer);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;

	void setupMesh();
public:

	Mesh() {}
	Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind, std::vector<Texture*> tex);

	void Draw(const Shader* shader);
};
