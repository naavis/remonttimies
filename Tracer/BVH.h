#pragma once
#include "Scene.h"
#include <memory>

class BVH {
public:
	BVH(std::shared_ptr<Scene> scene);
	BVH(std::shared_ptr<Scene>, unsigned int startIndex, unsigned int endIndex);
	bool IsLeafNode() const;
private:
	void BuildTree();
	unsigned int startIndex;
	unsigned int endIndex;
	std::unique_ptr<BVH> leftNode;
	std::unique_ptr<BVH> rightNode;
	std::shared_ptr<Scene> scene;
	bool isLeafNode;
};