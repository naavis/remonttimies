#pragma once
#include "../Scene.h"
#include <memory>
#include "RaycastResult.h"
#include "Ray.h"
#include "AABB.h"

class BVH {
public:
	BVH(std::shared_ptr<Scene> scene);
	BVH(std::shared_ptr<Scene>, unsigned int startIndex, unsigned int endIndex);
	RaycastResult Intersect(const Ray& ray) const;
	bool IsLeafNode() const;
	const AABB& GetAABB() const;
private:
	void BuildTree();
	void Initialize();
	RaycastResult IntersectTriangles(const Ray& ray) const;
	unsigned int startIndex;
	unsigned int endIndex;
	std::unique_ptr<BVH> leftNode;
	std::unique_ptr<BVH> rightNode;
	std::shared_ptr<Scene> scene;
	bool isLeafNode;
	AABB bbox;
};