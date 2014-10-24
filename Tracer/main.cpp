#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SceneFactory.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include <cstdio>
#include <string>
#include "OpenGLSceneManager.h"
#include "Renderer.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::printf("Please supply filename for scene file!");
		return -1;
	}

	std::string filename(argv[1]);
	std::shared_ptr<Scene> scene = SceneFactory::CreateFromFile(filename);
	const int width = 800;
	const int height = 600;
	float aspectRatio = static_cast<float>(width) / height;
	const float vFov = 70.0f;
	std::shared_ptr<Camera> camera(new Camera(glm::vec3(0.0, 0.0f, -2.0f), vFov, aspectRatio));
	Renderer renderer(scene, camera);
	Image image = renderer.Render(width, height);

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
	
	glEnable(GL_DEPTH_TEST);

	OpenGLSceneManager oglSceneManager;
	oglSceneManager.SetScene(scene);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		auto viewMatrix = camera->GetViewMatrix();
		auto projectionMatrix = camera->GetProjectionMatrix();
		oglSceneManager.Render(viewMatrix, projectionMatrix);
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		float speed = 0.01f;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			auto newPosition = camera->GetPosition() + glm::vec3(speed, 0.0f, 0.0f);
			camera->SetPosition(newPosition);
		} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			auto newPosition = camera->GetPosition() + glm::vec3(-speed, 0.0, 0.0f);
			camera->SetPosition(newPosition);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			auto newPosition = camera->GetPosition() + glm::vec3(0.0f, 0.0f, speed);
			camera->SetPosition(newPosition);
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			auto newPosition = camera->GetPosition() + glm::vec3(0.0f, 0.0f, -speed);
			camera->SetPosition(newPosition);
		}
	}

	glfwTerminate();

	return 0;
}