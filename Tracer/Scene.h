#pragma once
#include "Ray.h"
#include "RaycastResult.h"
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.h"

class Scene
{
public:
	typedef std::vector<Vertex> vertexList;
	typedef std::vector<glm::ivec3> triangleList;

	Scene(vertexList vertices, triangleList triangles);
	RaycastResult Intersect(const Ray& ray);
	const vertexList& GetVertices() const;
	const triangleList& GetTriangles() const;
	void SortTriangles(unsigned int startIndex, unsigned int endIndex, unsigned int axis);
private:
	vertexList vertices;
	triangleList triangles;
};