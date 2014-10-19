#include "Camera.h"
#include "Ray.h"
#include <glm/glm.hpp>

Camera::Camera(glm::vec3 position, float fieldOfViewDegrees)
	: fovRadians(glm::radians(fieldOfViewDegrees)),
	position(position),
	direction(glm::vec3(0.0f, 0.0f, 1.0f))
{
}

Ray Camera::GenerateRay(float x, float y) {
	glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(direction, globalUp));
	glm::vec3 up = glm::normalize(glm::cross(right, direction));
	float planeDistance = 1.0f / glm::tan(0.5f * fovRadians);
	Ray resultRay;
	resultRay.direction = glm::normalize(planeDistance * direction + x * right + y * up);
	resultRay.origin = position;
	return resultRay;
}

void Camera::LookAt(glm::vec3 point) {
	glm::vec3 direction = point - position;
	this->direction = glm::normalize(direction);
}

glm::vec3 Camera::GetDirection() const {
	return this->direction;
}
