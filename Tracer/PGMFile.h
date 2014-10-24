#pragma once
#include "Image.h"

class PGMFile {
public:
	static void Save(Image image, std::string filename);
};