#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SceneFactory.h"
#include "Scene.h"
#include "Camera.h"
#include <cstdio>
#include <string>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::printf("Please supply filename for scene file!");
		return -1;
	}

	std::string filename(argv[1]);
	Scene scene = SceneFactory::CreateFromFile(filename);
	Camera camera(glm::vec3(1.1, 0.0f, -2.0f), glm::vec3(0.0f, 0.0f, 1.0f), 80.0f);
	camera.LookAt(glm::vec3(0.0f, -0.2f, 0.0f));

	const int width = 800;
	const int height = 800;
	std::vector<int> image(width * height);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float localX = 2.0f * static_cast<float>(x) / width - 1.0f;
			float localY = 2.0f * static_cast<float>(y) / height - 1.0f;
			Ray ray = camera.GenerateRay(localX, -localY);
			RaycastResult result = scene.Intersect(ray);
			if (result.hit) {
				float maxDist = 5.0f;
				float visualDistance = glm::min(result.distance, maxDist);
				visualDistance = glm::max(0.0f, visualDistance);
				int intensity = static_cast<int>(255.0f - 255.0f * visualDistance / maxDist);
				image[width * y + x] = intensity;
			}
			else {
				image[width * y + x] = 0;
			}
		}
	}

	FILE* outFile = fopen("output.pgm", "w");
	std::fprintf(outFile, "P2\n");
	std::fprintf(outFile, "%i %i\n", width, height);
	std::fprintf(outFile, "255\n");
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			std::fprintf(outFile, "%i", image[width * y + x]);
			std::fprintf(outFile, " ");
		}
		std::fprintf(outFile, "\n");
	}

	std::fclose(outFile);
	return 0;
}