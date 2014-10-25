#include "PGMFile.h"
#include <cstdio>
#include <string>

void PGMFile::Save(Image image, std::string filename)
{
	Image normalizedImage = image.GetNormalized();
	std::string extension = ".pgm";
	std::string extendedFilename;
	if (extension.size() <= filename.size() &&
		std::equal(extension.rbegin(), extension.rend(), filename.rbegin())) {
		extendedFilename = filename;
	}
	else {
		extendedFilename = filename.append(extension);
	}
	FILE* outFile = std::fopen(extendedFilename.c_str(), "w");
	std::fprintf(outFile, "P2\n");
	std::fprintf(outFile, "%i %i\n", normalizedImage.GetWidth(), normalizedImage.GetHeight());
	std::fprintf(outFile, "255\n");
	for (unsigned int y = 0; y < normalizedImage.GetHeight(); ++y) {
		for (unsigned int x = 0; x < normalizedImage.GetWidth(); ++x) {
			std::fprintf(outFile, "%i", static_cast<int>(255.0f * normalizedImage.GetPixel(x, y)));
			std::fprintf(outFile, " ");
		}
		std::fprintf(outFile, "\n");
	}
	std::fclose(outFile);
}
