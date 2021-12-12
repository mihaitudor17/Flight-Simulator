#pragma once
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"

class Terrain
{
	unsigned int texture = 0;
public:
	Terrain(const std::string& TextureString);
	void Render();
};