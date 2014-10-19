#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SceneFactory.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include <cstdio>
#include <string>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::printf("Please supply filename for scene file!");
		return -1;
	}

	std::string filename(argv[1]);
	Scene scene = SceneFactory::CreateFromFile(filename);
	Camera camera(glm::vec3(1.1, 0.0f, -2.0f), 80.0f);
	camera.LookAt(glm::vec3(0.0f, -0.2f, 0.0f));

	const int width = 800;
	const int height = 800;
	Image image(width, height);

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
				float intensity = 255.0f - 255.0f * visualDistance / maxDist;
				image.SetPixel(x, y, intensity);
			}
			else {
				image.SetPixel(x, y, 0.0f);
			}
		}
	}

	FILE* outFile = fopen("output.pgm", "w");
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

	const char* vertexShaderSrc = "#version 150\nin vec2 position;\nvoid main()\n{\ngl_position = vec4(position, 0.0, 1.0);\n}\0";
	const char* fragmentShaderSrc = "#version150\nout vec4 outColor;\nvoid main()\n{\noutColor = vec4(1.0, 1.0, 1.0, 1.0);\n}\0";
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
	glCompileShader(vertexShader);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
	glCompileShader(fragmentShader);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	GLuint sceneVBO;
	glGenBuffers(1, &sceneVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sceneVBO);
	glBufferData(GL_ARRAY_BUFFER, scene.GetVertices().size() * sizeof(glm::vec3), &(scene.GetVertices()[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene.GetFaces().size() * sizeof(glm::ivec3), &(scene.GetFaces()[0]), GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window)) {
		glDrawElements(GL_TRIANGLES, scene.GetFaces().size(), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	glfwTerminate();

	return 0;
}