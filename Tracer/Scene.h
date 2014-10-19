#pragma once
#include "Ray.h"
#include "RaycastResult.h"
#include <glm/glm.hpp>
#include <vector>

class Scene
{
public:
	typedef std::vector<glm::vec3> vertexList;
	typedef std::vector<glm::ivec3> faceList;
	Scene(vertexList vertices, faceList faces);
	RaycastResult Intersect(Ray ray);
	const vertexList& GetVertices() const;
	const faceList& GetFaces() const;
private:
	vertexList vertices;
	faceList faces;
};