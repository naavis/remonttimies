#include "Image.h"

Image::Image(unsigned int width, unsigned int height)
	: width(width), height(height)
{
	imageContents.resize(width * height);
}

void Image::SetPixel(unsigned int x, unsigned int y, float intensity)
{
	imageContents[y * width + x] = intensity;
}

float Image::GetPixel(unsigned int x, unsigned int y) const
{
	return imageContents[y * width + x];
}

unsigned int Image::GetWidth() const
{
	return this->width;
}

unsigned int Image::GetHeight() const
{
	return this->height;
}