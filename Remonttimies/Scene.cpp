#include "Scene.h"
#include "Raytracing/Ray.h"
#include "Raytracing/RaycastResult.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <climits>
#include <algorithm>
#include "Vertex.h"

Scene::Scene(std::vector<Vertex> vertices, std::vector<glm::ivec3> triangles, materialList materials)
	: vertices(vertices), triangles(triangles), materials(materials)
{
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

const Material& Scene::GetMaterial(unsigned int materialIndex)
{
	return materials[materialIndex];
}