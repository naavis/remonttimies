#pragma once
#include <glm/glm.hpp>

typedef struct RaycastResult RaycastResult;

struct RaycastResult
{
	bool hit = false;
	float distance;
	glm::vec3 position;
	glm::vec3 barycentric;
};

