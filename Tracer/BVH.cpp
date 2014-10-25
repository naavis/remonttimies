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
	rightNode(nullptr)
{
	BuildTree();
}

BVH::BVH(std::shared_ptr<Scene> scene, unsigned int startIndex, unsigned int endIndex)
	: startIndex(startIndex),
	endIndex(endIndex),
	scene(scene),
	leftNode(nullptr),
	rightNode(nullptr)
{
	startIndex = glm::max(startIndex, 0u);
	endIndex = glm::min(endIndex, scene->GetFaces().size());
	BuildTree();
}

void BVH::BuildTree()
{
	unsigned int minAxis = 0;
	unsigned int minIndex = 0;
	unsigned int length = endIndex - startIndex;

	for (auto axis = 0u; axis < 3; ++axis) {
		scene->SortFaces(startIndex, endIndex, axis);
		AABB bbox(scene->GetFaces().cbegin() + startIndex, scene->GetFaces().cbegin() + endIndex, scene);
		// Create bounding boxes
		// Calculate SAH for each splitting plane
	}
	// Set children based on best splitting plane
	// Recurse
}
