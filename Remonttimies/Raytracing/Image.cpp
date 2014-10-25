#include "Image.h"
#include <limits>

Image::Image(unsigned int width, unsigned int height)
	: width(width), height(height)
{
	imageContents.resize(width * height);
}

void Image::SetPixel(unsigned int x, unsigned int y, glm::vec3 color)
{
	imageContents[y * width + x] = color;
}

glm::vec3 Image::GetPixel(unsigned int x, unsigned int y) const
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

float Image::GetMaximumValue() const
{
	float max = std::numeric_limits<float>::lowest();
	for (auto i = 0u; i < width * height; ++i) {
		for (auto j = 0u; j < 3; ++j) {
			if (imageContents[i][j] > max)
				max = imageContents[i][j];
		}
	}
	return max;
}

Image Image::GetNormalized() const
{
	Image newImage(this->GetWidth(), this->GetHeight());
	float max = this->GetMaximumValue();
	for (auto y = 0u; y < this->GetHeight(); ++y) {
		for (auto x = 0u; x < this->GetWidth(); ++x) {
			newImage.SetPixel(x, y, this->GetPixel(x, y) / max);
		}
	}

	return newImage;
}
