#include "Scene.h"
#include "Ray.h"
#include "RaycastResult.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <climits>
#include <algorithm>
#include "Vertex.h"

Scene::Scene(std::vector<Vertex> vertices, std::vector<glm::ivec3> faces)
	: vertices(vertices), faces(faces)
{
}

RaycastResult Scene::Intersect(const Ray& ray) {
	RaycastResult result;
	for (auto face : faces) {
		glm::vec3 v0 = vertices[face.x].position;
		glm::vec3 v1 = vertices[face.y].position;
		glm::vec3 v2 = vertices[face.z].position;
		glm::vec3 intersectionResult;
		bool hit = glm::intersectLineTriangle(ray.origin, ray.direction, v0, v1, v2, intersectionResult);
		if (hit) {
			glm::vec3 newPosition = ray.origin + intersectionResult.x * ray.direction;
			float candidateDistance = intersectionResult.x;
			if (candidateDistance < result.distance && candidateDistance > 0.0f) {
				result.distance = candidateDistance;
				result.position = newPosition;
				result.barycentric = glm::vec3(intersectionResult.y, intersectionResult.z, 1.0f - intersectionResult.y - intersectionResult.z);
				result.hit = true;
			}
		}
	}
	return result;
}

const Scene::vertexList& Scene::GetVertices() const {
	return vertices;
}

const Scene::triangleList& Scene::GetTriangles() const {
	return faces;
}

void Scene::SortTriangles(unsigned int startIndex, unsigned int endIndex, unsigned int axis)
{
	if (startIndex < 0)
		startIndex = 0;
	if (endIndex > GetTriangles().size())
		endIndex = GetTriangles().size();

	std::sort(faces.begin() + startIndex, faces.begin() + endIndex, [axis](glm::ivec3 a, glm::ivec3 b) {
		return a[axis] < b[axis];
	});
}