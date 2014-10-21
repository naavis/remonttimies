#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

class Camera
{
public:
	Camera(glm::vec3 position, float verticalFieldOfView, float aspectRatio);
	void SetPosition(glm::vec3 position);
	Ray GenerateRay(float x, float y);
	void LookAt(glm::vec3 point);
	glm::vec3 GetDirection() const;
	glm::vec3 GetPosition() const;
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewMatrix() const;
private:
	float vFov;
	glm::vec3 direction;
	glm::vec3 position;
	float aspectRatio;
};