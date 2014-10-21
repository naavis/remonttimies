#pragma once
#include "Scene.h"
#include <GL/glew.h>
#include <memory>

class OpenGLSceneManager {
public:
	OpenGLSceneManager();
	~OpenGLSceneManager();
	void SetScene(std::shared_ptr<Scene> scene);
	void Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
	void ClearBuffers();
	void DeleteShaders();
	void InitShaders();
	void PrintPossibleShaderError();
	void PrintPossibleShaderError(GLuint shaderID);
	std::shared_ptr<Scene> scene;
	GLuint sceneVBO;
	GLuint elementBO;
	GLuint shaderProgram;
	GLuint fragmentShader;
	GLuint vertexShader;
	GLuint viewMatrixID;
	GLuint projectionMatrixID;
};