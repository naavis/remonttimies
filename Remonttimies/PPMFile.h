#pragma once
#include "Raytracing/Image.h"

class PPMFile {
public:
	static void Save(Image image, std::string filename);
};