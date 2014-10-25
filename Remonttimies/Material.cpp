#include "Material.h"

Material::Material(glm::vec3 diffuseColor)
	: diffuseColor(diffuseColor)
{
}

glm::vec3 Material::GetDiffuseColor() const
{
	return diffuseColor;
}