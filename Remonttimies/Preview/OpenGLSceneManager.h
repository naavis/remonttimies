#pragma once
#include "../Scene.h"
#include <GL/glew.h>
#include <memory>

class OpenGLSceneManager {
public:
	OpenGLSceneManager();
	~OpenGLSceneManager();
	void SetScene(std::shared_ptr<Scene> scene);
	void Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

private:
	OpenGLSceneManager(const OpenGLSceneManager& other) {}
	OpenGLSceneManager& operator=(OpenGLSceneManager other) {}
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
	GLuint vertexAttribLoc;
	GLuint normalAttribLoc;
};