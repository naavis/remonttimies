#pragma once
#include <glm/glm.hpp>
#include <limits>

typedef struct RaycastResult RaycastResult;

struct RaycastResult
{
	bool hit = false;
	float distance = std::numeric_limits<float>::max();
	glm::vec3 position;
	glm::vec3 barycentric;
};

