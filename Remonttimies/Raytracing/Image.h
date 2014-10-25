#pragma once
#include <vector>
#include <glm/glm.hpp>

class Image
{
public:
	Image(unsigned int width, unsigned int height);
	void SetPixel(unsigned int x, unsigned int y, glm::vec3 color);
	glm::vec3 GetPixel(unsigned int x, unsigned int y) const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	float GetMaximumValue() const;
	Image GetNormalized() const;
private:
	unsigned int width;
	unsigned int height;
	std::vector<glm::vec3> imageContents;
};