#pragma once
#include "Scene.h"
#include <GL/glew.h>
#include <memory>

class OpenGLSceneManager {
public:
	OpenGLSceneManager();
	~OpenGLSceneManager();
	void SetScene(std::shared_ptr<Scene> scene);
	void Render();

private:
	void ClearBuffers();
	std::shared_ptr<Scene> scene;
	GLuint sceneVBO;
	GLuint elementBO;
};