#include "Scene.h"
#include "Raytracing/Ray.h"
#include "Raytracing/RaycastResult.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <climits>
#include <algorithm>
#include "Vertex.h"

Scene::Scene(std::vector<Vertex> vertices, std::vector<glm::ivec3> triangles)
	: vertices(vertices), triangles(triangles)
{
}

RaycastResult Scene::Intersect(const Ray& ray) {
	RaycastResult result;
	for (auto triangle : triangles) {
		glm::vec3 v0 = vertices[triangle.x].position;
		glm::vec3 v1 = vertices[triangle.y].position;
		glm::vec3 v2 = vertices[triangle.z].position;
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
	return triangles;
}

void Scene::SortTriangles(unsigned int startIndex, unsigned int endIndex, unsigned int axis)
{
	if (startIndex < 0)
		startIndex = 0;
	if (endIndex > GetTriangles().size())
		endIndex = GetTriangles().size();

	std::sort(triangles.begin() + startIndex, triangles.begin() + endIndex, [axis, this](glm::ivec3 a, glm::ivec3 b) {
		float aSum = 0.0f;
		float bSum = 0.0f;
		for (auto i = 0u; i < 3; ++i) {
			Vertex aVertex = this->GetVertices()[a[i]];
			Vertex bVertex = this->GetVertices()[b[i]];
			aSum += aVertex.position[axis];
			bSum += bVertex.position[axis];
		}
		return aSum < bSum;
	});
}