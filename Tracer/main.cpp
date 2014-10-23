#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SceneFactory.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include <cstdio>
#include <string>
#include "OpenGLSceneManager.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::printf("Please supply filename for scene file!");
		return -1;
	}

	std::string filename(argv[1]);
	std::shared_ptr<Scene> scene = SceneFactory::CreateFromFile(filename);
	const int width = 800;
	const int height = 800;
	float aspectRatio = static_cast<float>(width) / height;
	const float vFov = 80.0f;
	Camera camera(glm::vec3(1.1, 0.0f, -2.0f), vFov, aspectRatio);
	camera.LookAt(glm::vec3(0.0f, -0.2f, 0.0f));
	Image image(width, height);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float localX = 2.0f * static_cast<float>(x) / width - 1.0f;
			float localY = 2.0f * static_cast<float>(y) / height - 1.0f;
			Ray ray = camera.GenerateRay(localX, -localY);
			RaycastResult result = scene->Intersect(ray);
			if (result.hit) {
				float maxDist = 5.0f;
				float visualDistance = glm::min(result.distance, maxDist);
				visualDistance = glm::max(0.0f, visualDistance);
				float intensity = 255.0f - 255.0f * visualDistance / maxDist;
				image.SetPixel(x, y, intensity);
			}
			else {
				image.SetPixel(x, y, 0.0f);
			}
		}
	}

	FILE* outFile = std::fopen("output.pgm", "w");
	std::fprintf(outFile, "P2\n");
	std::fprintf(outFile, "%i %i\n", width, height);
	std::fprintf(outFile, "255\n");
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			std::fprintf(outFile, "%i", static_cast<int>(image.GetPixel(x, y)));
			std::fprintf(outFile, " ");
		}
		std::fprintf(outFile, "\n");
	}
	std::fclose(outFile);

	glfwInit();
	auto window = glfwCreateWindow(width, height, "Remonttimies", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	OpenGLSceneManager oglSceneManager;
	oglSceneManager.SetScene(scene);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		auto viewMatrix = camera.GetViewMatrix();
		auto projectionMatrix = camera.GetProjectionMatrix();
		oglSceneManager.Render(viewMatrix, projectionMatrix);
		glfwSwapBuffers(window);
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		float speed = 0.01f;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			auto newPosition = camera.GetPosition() + glm::vec3(speed, 0.0f, 0.0f);
			camera.SetPosition(newPosition);
		} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			auto newPosition = camera.GetPosition() + glm::vec3(-speed, 0.0, 0.0f);
			camera.SetPosition(newPosition);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			auto newPosition = camera.GetPosition() + glm::vec3(0.0f, 0.0f, speed);
			camera.SetPosition(newPosition);
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			auto newPosition = camera.GetPosition() + glm::vec3(0.0f, 0.0f, -speed);
			camera.SetPosition(newPosition);
		}
	}

	glfwTerminate();

	return 0;
}