#include "BVH.h"
#include "Scene.h"
#include <memory>

BVH::BVH(std::shared_ptr<Scene> scene)
	: startIndex(0),
	endIndex(scene->GetFaces().size()),
	scene(scene),
	leftNode(nullptr),
	rightNode(nullptr)
{

}

BVH::BVH(std::shared_ptr<Scene>, unsigned int startIndex, unsigned int endIndex)
	: startIndex(startIndex),
	endIndex(endIndex),
	scene(scene),
	leftNode(nullptr),
	rightNode(nullptr)
{
	startIndex = glm::max(0u, startIndex);
	endIndex = glm::min(endIndex, scene->GetFaces().size());
}
