#include "Renderer.h"
#include "Image.h"
#include <memory>
#include "Scene.h"
#include "Camera.h"

Renderer::Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera)
	: scene(scene), camera(camera)
{

}

Image Renderer::Render(unsigned int width, unsigned int height)
{
	Image image(width, height);
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			float localX = 2.0f * static_cast<float>(x) / width - 1.0f;
			float localY = 2.0f * static_cast<float>(y) / height - 1.0f;
			Ray ray = camera->GenerateRay(localX, -localY);
			RaycastResult result = scene->Intersect(ray);
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

	return image;
}
