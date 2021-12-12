#include "PointLight.h"

PointLight::PointLight(const glm::vec3& pos):
	position(pos)
{
}

void PointLight::SetPosition(const glm::vec3& pos)
{
	position = pos;
}

glm::vec3 PointLight::GetPosition()
{
	return position;
}

glm::mat4 PointLight::ViewMatrix()
{
	return glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 PointLight::LightSpaceMatrix()
{
	return Projection * ViewMatrix();
}
