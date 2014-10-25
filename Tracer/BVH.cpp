#include "BVH.h"
#include "Scene.h"
#include <memory>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "AABB.h"

BVH::BVH(std::shared_ptr<Scene> scene)
	: startIndex(0),
	endIndex(scene->GetFaces().size()),
	scene(scene),
	leftNode(nullptr),
	rightNode(nullptr),
	isLeafNode(false)
{
	Initialize();
}

BVH::BVH(std::shared_ptr<Scene> scene, unsigned int startIndex, unsigned int endIndex)
	: startIndex(startIndex),
	endIndex(endIndex),
	scene(scene),
	leftNode(nullptr),
	rightNode(nullptr),
	isLeafNode(false)
{
	Initialize();
}

void BVH::Initialize() {
	bbox = AABB(scene->GetFaces().cbegin() + startIndex, scene->GetFaces().cbegin() + endIndex, scene);
	const unsigned int trianglesInLeaf = 16;
	startIndex = glm::max(startIndex, 0u);
	endIndex = glm::min(endIndex, scene->GetFaces().size());
	if (endIndex - startIndex > trianglesInLeaf) {
		BuildTree();
	}
	else {
		isLeafNode = true;
	}
}

void BVH::BuildTree()
{
	unsigned int minAxis = 0;
	unsigned int minIndex = 0;
	float minSurfaceAreaHeuristic = std::numeric_limits<float>::max();
	unsigned int length = endIndex - startIndex;

	for (auto axis = 0u; axis < 3; ++axis) {
		scene->SortFaces(startIndex, endIndex, axis);
		AABB totalBoundingBox(scene->GetFaces().cbegin() + startIndex, scene->GetFaces().cbegin() + endIndex, scene);
		float* surfaceAreas = new float[length];
		float* reverseSurfaceAreas = new float[length];
		AABB boundingBox;
		AABB reverseBoundingBox;
		for (auto splittingPlane = 0u; splittingPlane < length; ++splittingPlane) {
			unsigned int triangleIndex = startIndex + splittingPlane;
			boundingBox.Add(scene->GetFaces()[triangleIndex], scene);
			surfaceAreas[splittingPlane] = boundingBox.GetSurfaceArea();

			unsigned int reverseTriangleIndex = endIndex - 1 - splittingPlane;
			reverseBoundingBox.Add(scene->GetFaces()[reverseTriangleIndex], scene);
			reverseSurfaceAreas[length - 1 - splittingPlane] = reverseBoundingBox.GetSurfaceArea();
		}

		for (auto splittingPlane = 0u; splittingPlane < length; ++splittingPlane) {
			float surfaceAreaHeuristic = splittingPlane * surfaceAreas[splittingPlane] +
				(length - splittingPlane) * reverseSurfaceAreas[splittingPlane];
			if (surfaceAreaHeuristic < minSurfaceAreaHeuristic) {
				minSurfaceAreaHeuristic = surfaceAreaHeuristic;
				minAxis = axis;
				minIndex = splittingPlane;
			}
		}

		delete[] surfaceAreas;
		delete[] reverseSurfaceAreas;
	}

	if (minIndex == 0) {
		isLeafNode = true;
		return;
	}

	minIndex += startIndex;

	scene->SortFaces(startIndex, endIndex, minAxis);
	this->leftNode = std::unique_ptr<BVH>(new BVH(scene, startIndex, minIndex));
	this->rightNode = std::unique_ptr<BVH>(new BVH(scene, minIndex + 1, endIndex));
}

bool BVH::IsLeafNode() const
{
	return isLeafNode;
}

RaycastResult BVH::Intersect(Ray ray) const
{

}
