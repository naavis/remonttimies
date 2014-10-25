#pragma once
#include <glm/glm.hpp>
#include "RaycastResult.h"
#include "Ray.h"
#include <vector>
#include "../Scene.h"
#include <memory>

class AABB {
public:
	typedef std::vector<glm::ivec3>::const_iterator triangleIterator;
	AABB();
	AABB(triangleIterator startIter, triangleIterator endIter, const std::shared_ptr<Scene> scene);
	void Add(Vertex vertex);
	void Add(const glm::ivec3 triangle, const std::shared_ptr<Scene> scene);
	void Add(triangleIterator startIter, triangleIterator endIter, const std::shared_ptr<Scene> scene);
	RaycastResult Intersect(const Ray& ray) const;
	float GetSurfaceArea() const;
	void InitializeCorners();
private:
	glm::vec3 minCorner;
	glm::vec3 maxCorner;
};