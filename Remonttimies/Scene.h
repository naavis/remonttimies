#pragma once
#include "Raytracing/Ray.h"
#include "Raytracing/RaycastResult.h"
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.h"
#include "Material.h"

class Scene
{
public:
	typedef std::vector<Vertex> vertexList;
	typedef std::vector<glm::ivec3> triangleList;
	typedef std::vector<Material> materialList;

	Scene(vertexList vertices, triangleList triangles, materialList materials);
	RaycastResult Intersect(const Ray& ray);
	const vertexList& GetVertices() const;
	const triangleList& GetTriangles() const;
	void SortTriangles(unsigned int startIndex, unsigned int endIndex, unsigned int axis);
	const Material& GetMaterial(unsigned int index);
private:
	vertexList vertices;
	triangleList triangles;
	materialList materials;
};