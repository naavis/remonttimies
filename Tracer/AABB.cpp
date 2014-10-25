#include "AABB.h"
#include <glm/glm.hpp>
#include "RaycastResult.h"
#include "Ray.h"
#include <limits>
#include <algorithm>
#include "Vertex.h"
#include <memory>

void AABB::Add(Vertex vertex)
{
	minCorner = glm::min(vertex.position, minCorner);
	maxCorner = glm::max(vertex.position, maxCorner);
}

AABB::AABB(AABB::triangleIterator startIter, AABB::triangleIterator endIter, const std::shared_ptr<Scene> scene)
{
	this->Add(startIter, endIter, scene);
}


void AABB::Add(AABB::triangleIterator startIter, AABB::triangleIterator endIter, const std::shared_ptr<Scene> scene)
{
	for (AABB::triangleIterator iter = startIter; iter != endIter; ++iter) {
		for (auto i = 0u; i < 3; ++i) {
			auto vertexIndex = (*iter)[i];
			Vertex vertex = scene->GetVertices()[vertexIndex];
			this->Add(vertex);
		}
	}
}

RaycastResult AABB::Intersect(Ray ray) const
{
	float tStart = std::numeric_limits<float>::lowest();
	float tEnd = std::numeric_limits<float>::max();
	RaycastResult result;

	glm::vec3 inverseDirection = 1.0f / ray.direction;

	for (auto axis = 0u; axis < 3; ++axis) {
		if (ray.direction[axis] == 0.0f &&
			(ray.origin[axis] < minCorner[axis] || ray.origin[axis] > maxCorner[axis])) {
			return result;
		}
		float tNear = (minCorner[axis] - ray.origin[axis]) * inverseDirection[axis];
		float tFar = (maxCorner[axis] - ray.origin[axis]) * inverseDirection[axis];
		if (tNear > tFar)
			std::swap(tNear, tFar);
		tStart = glm::max(tNear, tStart);
		tEnd = glm::min(tFar, tEnd);
	}

	if (tStart > tEnd || tEnd < 0.0f) {
		return result;
	}

	if (tStart > 0.0f) {
		result.hit = true;
		result.distance = tStart;
		result.position = ray.origin + result.distance * ray.direction;
	}
	else {
		result.hit = true;
		result.distance = 0.0f;
		result.position = ray.origin;
	}

	return result;
}