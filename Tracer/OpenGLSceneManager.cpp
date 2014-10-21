#include "OpenGLSceneManager.h"
#include <memory>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

OpenGLSceneManager::OpenGLSceneManager()
	: sceneVBO(0), elementBO(0)
{
	InitShaders();
}

void OpenGLSceneManager::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(this->viewMatrixID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(this->projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
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
	this->DeleteShaders();
}

void OpenGLSceneManager::ClearBuffers() {
	if (this->scene) {
		glDeleteBuffers(1, &sceneVBO);
		glDeleteBuffers(1, &elementBO);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void OpenGLSceneManager::InitShaders()
{
	const char* vertexShaderSrc =
		"#version 150\n"
		"in vec3 position;"
		"uniform mat4 viewMatrix;"
		"uniform mat4 projectionMatrix;"
		"void main() {" 
		"	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);"
		"}";
	const char* fragmentShaderSrc =
		"#version 150\n"
		"out vec4 outColor;"
		"void main() {"
		"	outColor = vec4(1.0, 1.0, 1.0, 1.0);"
		"}";
	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
	glCompileShader(vertexShader);
	PrintPossibleShaderError(vertexShader);
	this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
	glCompileShader(fragmentShader);
	PrintPossibleShaderError(fragmentShader);
	this->shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	this->viewMatrixID = glGetUniformLocation(shaderProgram, "viewMatrix");
	this->projectionMatrixID = glGetUniformLocation(shaderProgram, "projectionMatrix");
}

void OpenGLSceneManager::DeleteShaders()
{
	glDeleteProgram(this->shaderProgram);
	glDeleteShader(this->vertexShader);
	glDeleteShader(this->fragmentShader);
}

void OpenGLSceneManager::PrintPossibleShaderError(GLuint shaderID)
{
	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shaderID, 512, NULL, buffer);
		std::printf(buffer);
	}
	else {
		std::printf("Shader %i compiled successfully!\n", shaderID);
	}
}
