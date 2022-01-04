#include "Airplane.h"
#include <gtx/euler_angles.hpp>
#include <glfw3.h>

static Model* AirplaneModel = nullptr;

Airplane::Airplane()
{
	InitModel();
	model = AirplaneModel;
}

Airplane::Airplane(const glm::vec3& pos):
	position(pos)
{
	InitModel();
	model = AirplaneModel;
}

glm::vec3 Airplane::Position() const
{
	return position;
}

void Airplane::InitModel()
{
	if (!AirplaneModel)
	{
		AirplaneModel = new Model("../models/boeing_747/scene.gltf");
	}
}

void Airplane::Break(const float& deltaTime)
{
	if (position.y < 1 && position.y >= 0)
	{
		if (glm::length(velocity) <= 0.5f)
		{
			velocity = glm::vec3(0.0f);
			return;
		}
		velocity -= deltaTime * velocity * 0.5f;
	}
}

void Airplane::ModifyThrottle(const float& deltaTime, const int& dir)
{
	throttle += dir * deltaTime * throttleIncrement;
	if (throttle < 0)
		throttle = 0;
	if (throttle > throttleLimit)
		throttle = throttleLimit;
}

void Airplane::ModifyYaw(const float& deltaTime, const int& dir)
{
	//glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
	float speedratio = glm::length(velocity) / 11.0f;
	yaw += deltaTime * -dir * 30.0f * speedratio;
	rotation.z += deltaTime * dir * glm::radians(30.0f * speedratio);
}

void Airplane::ModifyPitch(const float& deltaTime, const int& dir)
{
	
	float speedratio = glm::length(velocity) / 11.0f;
	pitch += deltaTime * -dir * pitchIncrement * speedratio;
	rotation.x -= deltaTime * dir * glm::radians(pitchIncrement * speedratio);
}

void Airplane::ModifyRoll(const float& deltaTime, const int& dir)
{
	float speedratio = glm::length(velocity) / 11.0f;
	roll += deltaTime * dir * rollIncrement * speedratio;
	rotation.y += deltaTime * dir * glm::radians(30.0f * speedratio);
}

void Airplane::SetScale(const glm::vec3& val)
{
	scale = val;
}

void Airplane::AddForce(const glm::vec3& force)
{
	velocity += force;
}

void Airplane::Draw(const Shader* shader)
{
	glm::mat4 model_mat(1.0f);
	model_mat = glm::translate(model_mat, position);
	model_mat = glm::translate(model_mat, { 0.0f, 1.3f, 0.0f });
	model_mat = glm::rotate(model_mat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model_mat = glm::rotate(model_mat, rotation.x, {1.0f, 0.0f, 0.0f});
	model_mat = glm::rotate(model_mat, rotation.y, {0.0f, 1.0f, 0.0f});
	model_mat = glm::rotate(model_mat, rotation.z, {0.0f, 0.0f, 1.0f});
	model_mat = glm::scale(model_mat, scale);


	shader->Use();
	shader->SetMat4("model", model_mat);
	model->Draw(shader);
}

void Airplane::Update(const float& deltaTime)
{
	// Calculate the new forward vector
	this->forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->forward.y = sin(glm::radians(pitch));
	this->forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->forward = glm::normalize(this->forward);
	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(forward, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, forward));
	//up.x = cos(pitch);
	//throttle

	AddForce(forward * deltaTime * throttle);

	ApplyLift(deltaTime);
	ApplyDrag(deltaTime);
	ApplyGravity(deltaTime);


	glm::vec2 flatVelocity(velocity.x, velocity.z);
	if (glm::length(flatVelocity) > speedLimit)
	{
		velocity = glm::normalize(velocity) * speedLimit;
	}
	
	if (position.y <= 0.1f)
	{
		position.y = 0.1;
		if (velocity.y < 0)
			velocity.y = 0;
	}
	
	position += velocity * deltaTime;
	if (position.y > hightLimit)
	{
		position.y = hightLimit;
	}
}

void Airplane::ApplyGravity(const float& deltaTime)
{
	velocity -= glm::vec3(0.0f, 9.8f * 9.8f, 0.0f) * deltaTime;
}

void Airplane::ApplyLift(const float& deltaTime)
{
	float wingArea = 12;
	float airDensity = 1.2f;
	float liftCoefficient = 0.13f;

	glm::vec2 flatVelocity(velocity.x, velocity.z);
	float speed = glm::length(flatVelocity);
	//std::cout << speed << std::endl;
	float lift = (liftCoefficient * wingArea * airDensity * speed * speed ) / 2.0f;

	AddForce(up * lift *deltaTime);
}

void Airplane::ApplyDrag(const float& deltaTime)
{
	float area = 0.3;
	float airDensity = 1.2f;
	float dragCoefficient = 0.2f;
	glm::vec2 flatVelocity(velocity.x, velocity.z);
	float speed = glm::length(flatVelocity);
	float drag = dragCoefficient * (airDensity * speed * speed) / 2.0f * area * deltaTime;
	AddForce(velocity * -drag);
}

void Airplane::ProcessInput(const float& deltaTime)
{
	
}
