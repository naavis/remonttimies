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
	const vertexList& GetVertices() const;
	const triangleList& GetTriangles() const;
	void SortTriangles(unsigned int startIndex, unsigned int endIndex, unsigned int axis);
	const Material& GetMaterial(unsigned int materialIndex);
private:
	vertexList vertices;
	triangleList triangles;
	materialList materials;
};