#pragma once
#include <glm/glm.hpp>

class Material {
public:
	Material(glm::vec3 diffuseColor);
	glm::vec3 GetDiffuseColor() const;
private:
	glm::vec3 diffuseColor;
};