#include "OpenGLSceneManager.h"
#include <memory>

OpenGLSceneManager::OpenGLSceneManager()
	: sceneVBO(0), elementBO(0)
{
}

void OpenGLSceneManager::Render()
{
	glBindBuffer(GL_ARRAY_BUFFER, sceneVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBO);
	if (this->scene) {
		glDrawElements(GL_TRIANGLES, scene->GetFaces().size(), GL_UNSIGNED_INT, 0);
	}
}

void OpenGLSceneManager::SetScene(std::shared_ptr<Scene> scene)
{
	this->ClearBuffers();
	this->scene = scene;
	glGenBuffers(1, &sceneVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sceneVBO);
	glBufferData(GL_ARRAY_BUFFER, scene->GetVertices().size() * sizeof(glm::vec3), &(scene->GetVertices()[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &elementBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene->GetFaces().size() * sizeof(glm::ivec3), &(scene->GetFaces()[0]), GL_STATIC_DRAW);
}

OpenGLSceneManager::~OpenGLSceneManager()
{
	this->ClearBuffers();
}

void OpenGLSceneManager::ClearBuffers() {
	if (this->scene) {
		glDeleteBuffers(1, &sceneVBO);
		glDeleteBuffers(1, &elementBO);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}