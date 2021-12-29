#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <vector>

class Model
{
public:
	std::vector<Mesh*> meshes;
	std::vector<Texture*> textures_loaded;
	std::string directory;

	void LoadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	Model(std::string const& path)
	{
		LoadModel(path);
	}
	void Draw(const Shader* shader);
};