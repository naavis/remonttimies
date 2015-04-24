#include "Renderer.h"
#include "Image.h"
#include <memory>
#include "../Scene.h"
#include "../Camera.h"
#include "BVH.h"
#include "Utilities.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

Renderer::Renderer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera)
	: scene(scene), camera(camera)
{
	bvhTree = std::shared_ptr<BVH>(new BVH(scene));
}

Image Renderer::Render(unsigned int width, unsigned int height)
{
	Image image(width, height);
	int samplesPerPixel = 16;
	int totalBounces = 3;

	// Hard-coded light vector
	glm::vec3 lightVector = glm::normalize(glm::vec3(0.2f, -0.3f, 0.5f));
	glm::vec3 lightEmission = glm::vec3(50.0f);

	#pragma omp parallel for
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			glm::vec3 radianceSum = glm::vec3(0.0f);
			for (int sample = 0; sample < samplesPerPixel; ++sample) {
				// Calculate pixel coordinates
				float offsetX = static_cast<float>(rand()) / RAND_MAX - 0.5f;
				float offsetY = static_cast<float>(rand()) / RAND_MAX - 0.5f;
				float localX = 2.0f * (static_cast<float>(x) + offsetX) / width - 1.0f;
				float localY = 2.0f * (static_cast<float>(y) + offsetY) / height - 1.0f;

				glm::vec3 currentContribution = glm::vec3(1.0f);

				// First path segment
				Ray currentRay = camera->GenerateRay(localX, -localY);
				
				for (int bounce = 0; bounce < totalBounces + 1; ++bounce) {
					RaycastResult result = bvhTree->Intersect(currentRay);
					if (!result.hit)
					{
						// Ray does not hit anything, path terminates
						break;
					}

					glm::vec3 hitPoint = result.position - 5.0f * glm::epsilon<float>() * currentRay.direction;
					glm::vec3 bounceRadiance = glm::vec3(0.0f);

					Ray shadowRay;
					shadowRay.origin = hitPoint;
					shadowRay.direction = -lightVector;
					auto shadowRayResult = bvhTree->Intersect(shadowRay);
					if (!shadowRayResult.hit) {
						// Shadow ray did not hit geometry -> hits the directional light
						float thetaI = glm::max(0.0f, glm::dot(result.normal, shadowRay.direction));
						// Light PDF equals 1.0 with directional light, so it is omitted
						bounceRadiance += thetaI * lightEmission / glm::pi<float>();
					}

					glm::vec3 albedo = glm::pow(scene->GetMaterial(result.materialIndex).GetDiffuseColor(), glm::vec3(2.2f));
					radianceSum += albedo * currentContribution * bounceRadiance;
					currentContribution *= albedo;

					// Generate next path segment
					glm::mat3 basis = formBasis(result.normal);
					glm::vec2 diskPoint = toUnitDisk(glm::vec2(static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX));
					currentRay.origin = hitPoint;
					currentRay.direction = basis * glm::vec3(diskPoint.x, diskPoint.y, glm::sqrt(1.0f - glm::dot(diskPoint, diskPoint)));
					currentRay.direction = glm::normalize(currentRay.direction);
				}
			}

			radianceSum = radianceSum / static_cast<float>(samplesPerPixel);
			image.SetPixel(x, y, glm::pow(radianceSum, glm::vec3(0.45f)));
		}
	}

	return image;
}
