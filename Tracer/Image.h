#pragma once
#include <vector>

class Image
{
public:
	Image(unsigned int width, unsigned int height);
	void SetPixel(unsigned int x, unsigned int y, float intensity);
	float GetPixel(unsigned int x, unsigned int y) const;
private:
	unsigned int width;
	unsigned int height;
	std::vector<float> imageContents;
};