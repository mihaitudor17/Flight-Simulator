#pragma once
#include "Model.h"
class Airplane
{
	Model* model = nullptr;
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp = glm::vec3(0.0f,1.0f,0.0f);

	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	
	float speedLimit = 30.0f;

	float hightLimit = 200.0f;

	float pitch = 0.0f;
	float yaw = 90.0f;
	float roll = 0.0f;

	float throttle = 0.0f;
	float throttleIncrement = 8.0f;
	float throttleLimit = 90.0f;

	float rollIncrement = 30.0f;
	float pitchIncrement = 30.0f;



public:
	Airplane();
	Airplane(const glm::vec3&);

	glm::vec3 Position() const;

	void InitModel();

	void Break(const float& deltaTime);

	void ModifyThrottle(const float& deltaTime, const int& dir);
	void ModifyYaw(const float& deltaTime, const int& dir);
	void ModifyPitch(const float& deltaTime, const int& dir);

	void ModifyRoll(const float& deltaTime, const int& dir);

	void SetScale(const glm::vec3&);
	void AddForce(const glm::vec3&);
	void Draw(const Shader*);
	void Update(const float&);

	void ApplyGravity(const float& deltaTime);
	void ApplyLift(const float& deltaTime);
	void ApplyDrag(const float& deltaTime);

	void ProcessInput(const float&);
};