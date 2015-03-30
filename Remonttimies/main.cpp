#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SceneFactory.h"
#include "Scene.h"
#include "Camera.h"
#include "Raytracing/Image.h"
#include <cstdio>
#include <string>
#include "Preview/OpenGLSceneManager.h"
#include "Raytracing/Renderer.h"
#include "PPMFile.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::printf("Please supply filename for scene file!");
		return -1;
	}

	std::string filename(argv[1]);
	std::printf("Loading scene from file... ");
	std::shared_ptr<Scene> scene = SceneFactory::CreateFromFile(filename);
	std::printf("Done!\n");

	const int width = 1280;
	const int height = 768;

	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, 0);
	auto window = glfwCreateWindow(width, height, "Remonttimies", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	
	glEnable(GL_DEPTH_TEST);

	OpenGLSceneManager oglSceneManager;
	oglSceneManager.SetScene(scene);

	float aspectRatio = static_cast<float>(width) / height;
	const float vFov = 70.0f;
	std::shared_ptr<Camera> camera(new Camera(glm::vec3(0.0, 0.0f, -2.0f), vFov, aspectRatio));

	std::printf("Generating the Bounding Volume Hierarchy... ");
	Renderer renderer(scene, camera);
	std::printf("Done!\n");

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
			camera->MoveLocalX(-speed);
		} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera->MoveLocalX(speed);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera->MoveLocalZ(-speed);
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera->MoveLocalZ(speed);
		}

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			camera->MoveLocalY(speed);
		}
		else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			camera->MoveLocalY(-speed);
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			camera->Pitch(-speed);
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			camera->Pitch(speed);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			camera->Yaw(speed);
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			camera->Yaw(-speed);
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			std::printf("Rendering... ");
			Image image = renderer.Render(width, height);
			std::printf("Done!\n");
			std::printf("Saving to file... ");
			PPMFile::Save(image, "output");
			std::printf("Done!\n");
		}
	}

	glfwTerminate();

	return 0;
}