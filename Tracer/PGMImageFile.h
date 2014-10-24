#pragma once
#include "Image.h"

class PGMImageFile {
public:
	static void Save(Image image, std::string filename);
};