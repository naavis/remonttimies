#pragma once
#include "Raytracing/Image.h"

class PGMFile {
public:
	static void Save(Image image, std::string filename);
};