#pragma once
#include <glew.h>
#include <string>

#include <string>

class Texture
{
	unsigned int id;
	std::string type;
	std::string fileName;

public:

	Texture(const std::string&);

	unsigned int GetID();
	void SetType(const std::string&);
	std::string GetType();
	std::string GetFileName();

	static unsigned int CreateTexture(const std::string& texturePath);
};