#include "Camera.h"
#include "Ray.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

Camera::Camera(glm::vec3 position, float verticalFieldOfView, float aspectRatio)
	: vFov(verticalFieldOfView),
	position(position),
	direction(glm::vec3(0.0f, 0.0f, 1.0f)),
	aspectRatio(aspectRatio)
{
}

Ray Camera::GenerateRay(float x, float y) {
	glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(direction, globalUp));
	glm::vec3 up = glm::normalize(glm::cross(right, direction));
	float planeDistance = 1.0f / glm::tan(0.5f * glm::radians(vFov));
	Ray resultRay;
	resultRay.direction = glm::normalize(planeDistance * direction + aspectRatio * x * right + y * up);
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

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(this->vFov, aspectRatio, 0.001f, 50.0f);
}

void Camera::SetPosition(glm::vec3 position)
{
	this->position = position;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(this->position, this->position + this->direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::GetPosition() const
{
	return this->position;
}

void Camera::MoveLocalZ(float amount)
{
	SetPosition(GetPosition() - amount * GetDirection());
}

void Camera::MoveLocalY(float amount)
{
	auto glocalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	auto right = glm::cross(GetDirection(), glocalUp);
	auto localUp = glm::normalize(glm::cross(right, GetDirection()));
	SetPosition(GetPosition() + amount * localUp);
}

void Camera::MoveLocalX(float amount)
{
	auto localUp = glm::vec3(0.0f, 1.0f, 0.0f);
	auto right = glm::normalize(glm::cross(GetDirection(), localUp));
	SetPosition(GetPosition() + amount * right);
}

void Camera::Pitch(float amount)
{
	auto glocalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	auto right = glm::cross(GetDirection(), glocalUp);
	auto rotationMatrix = glm::rotate(50.0f * amount, right);
	glm::vec4 newDirection = rotationMatrix * glm::vec4(GetDirection(), 1.0f);
	this->direction = glm::vec3(newDirection.x, newDirection.y, newDirection.z);
}

void Camera::Yaw(float amount)
{
	auto rotationMatrix = glm::rotate(50.0f * amount, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 newDirection = rotationMatrix * glm::vec4(GetDirection(), 1.0f);
	this->direction = glm::vec3(newDirection.x, newDirection.y, newDirection.z);
}
