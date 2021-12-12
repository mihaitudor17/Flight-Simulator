#pragma once
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class PointLight
{
	glm::vec3 position = { 0,0,0 };
	
	float near_plane = 0.1f, far_plane = 3000.0f;
	glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
public:
	PointLight() = default;
	PointLight(const glm::vec3& pos);

	void SetPosition(const glm::vec3& pos);
	glm::vec3 GetPosition();

	glm::mat4 ViewMatrix();
	glm::mat4 LightSpaceMatrix();
	//glm::mat4 View = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	//glm::mat4 SpaceMatrix = lightProjection * lightView;

	//glm::
};