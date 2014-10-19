#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

class Camera
{
public:
	Camera(glm::vec3 position, float fieldOfViewDegrees);
	Ray GenerateRay(float x, float y);
	void LookAt(glm::vec3 point);
	glm::vec3 GetDirection() const;
private:
	float fovRadians;
	glm::vec3 direction;
	glm::vec3 position;
};