#pragma once
#include "Image.h"
#include <memory>
#include "Scene.h"
#include "Camera.h"

class Renderer {
public:
	Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
	Image Render(unsigned int width, unsigned int height);
private:
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Camera> camera;
};