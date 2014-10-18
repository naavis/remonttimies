#pragma once
#include <glm/glm.hpp>

typedef struct Ray Ray;

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};

