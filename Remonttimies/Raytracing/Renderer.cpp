#include "Renderer.h"
#include "Image.h"
#include <memory>
#include "../Scene.h"
#include "../Camera.h"
#include "BVH.h"

Renderer::Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera)
	: scene(scene), camera(camera)
{
	bvhTree = std::shared_ptr<BVH>(new BVH(scene));
}

Image Renderer::Render(unsigned int width, unsigned int height)
{
	Image image(width, height);
	#pragma omp parallel for
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float localX = 2.0f * static_cast<float>(x) / width - 1.0f;
			float localY = 2.0f * static_cast<float>(y) / height - 1.0f;
			Ray ray = camera->GenerateRay(localX, -localY);
			RaycastResult result = bvhTree->Intersect(ray);
			glm::vec3 sunLight = glm::normalize(glm::vec3(0.2f, -0.3f, 0.5f));
			if (result.hit) {
				Ray shadowRay;
				shadowRay.origin = result.position;
				shadowRay.direction = -sunLight;
				auto shadowRayResult = bvhTree->Intersect(shadowRay);
				float shadowFactor = 1.0f;
				if (shadowRayResult.hit && shadowRayResult.distance < 100.0f)
					shadowFactor = 0.1f;
				glm::vec3 materialColor = glm::pow(scene->GetMaterial(result.materialIndex).GetDiffuseColor(), glm::vec3(2.2f));
				auto shade = glm::max(0.0f, glm::dot(result.normal, -sunLight));
				auto finalColor = glm::pow(shadowFactor * shade * materialColor, glm::vec3(0.45f));
				image.SetPixel(x, y, finalColor);
			}
			else {
				image.SetPixel(x, y, glm::vec3(0.0f));
			}
		}
	}

	return image;
}
