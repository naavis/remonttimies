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
	if (scene == nullptr) {
		std::printf("Exiting...\n");
		return -1;
	}
	std::printf("Done!\n");

	const int width = 1280;
	const int height = 768;

	float aspectRatio = static_cast<float>(width) / height;
	const float vFov = 45.0f;
	std::shared_ptr<Camera> camera(new Camera(glm::vec3(0.0, 0.0f, -2.0f), vFov, aspectRatio));

	std::printf("Generating the Bounding Volume Hierarchy... ");
	Renderer renderer(scene, camera);
	std::printf("Done!\n");

	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, 0);
	auto window = glfwCreateWindow(width, height, "Remonttimies", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	
	glEnable(GL_DEPTH_TEST);

	OpenGLSceneManager oglSceneManager;
	oglSceneManager.SetScene(scene);

	std::vector<float> imageBuffer;
	bool showRenderedImage = false;

	// This is used to prevent multiple key press events when starting rendering or hiding the rendered image
	bool renderKeyDown = false;

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		auto viewMatrix = camera->GetViewMatrix();
		auto projectionMatrix = camera->GetProjectionMatrix();
		oglSceneManager.Render(viewMatrix, projectionMatrix);

		if (showRenderedImage)
			glDrawPixels(width, height, GL_RGB, GL_FLOAT, (const GLvoid*)&imageBuffer[0]);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		float moveSpeed = 0.05f;
		float rotationSpeed = 0.01f;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera->MoveLocalX(-moveSpeed);
		} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera->MoveLocalX(moveSpeed);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera->MoveLocalZ(-moveSpeed);
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera->MoveLocalZ(moveSpeed);
		}

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			camera->MoveLocalY(moveSpeed);
		}
		else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			camera->MoveLocalY(-moveSpeed);
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			camera->Pitch(-rotationSpeed);
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			camera->Pitch(rotationSpeed);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			camera->Yaw(rotationSpeed);
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			camera->Yaw(-rotationSpeed);
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
			renderKeyDown = false;

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			if (renderKeyDown == false) {
				renderKeyDown = true;

				if (showRenderedImage == false) {
					std::printf("Rendering...\n");
					Image image = renderer.Render(width, height);
					std::printf("Done!\n");
					std::printf("Saving to file... ");
					PPMFile::Save(image, "output");
					std::printf("Done!\n");

					// Convert Image object to buffer for glDrawPixels
					imageBuffer.clear();
					auto imageNormalized = image.GetNormalized();
					for (int y = 0; y < height; ++y) {
						for (int x = 0; x < width; ++x) {
							auto pixel = imageNormalized.GetPixel(x, height - y - 1);
							imageBuffer.push_back(pixel.r);
							imageBuffer.push_back(pixel.g);
							imageBuffer.push_back(pixel.b);
						}
					}
				}

				showRenderedImage = !showRenderedImage;
			}
		}
	}

	glfwTerminate();

	return 0;
}