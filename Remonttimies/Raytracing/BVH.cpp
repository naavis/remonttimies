#include "BVH.h"
#include "../Scene.h"
#include <memory>
#include <glm/glm.hpp>
#include "../Vertex.h"
#include "AABB.h"
#include "RaycastResult.h"
#include <glm/gtx/intersect.hpp>

BVH::BVH(std::shared_ptr<Scene> scene)
	: startIndex(0),
	endIndex(scene->GetTriangles().size()),
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
	const unsigned int trianglesInLeaf = 16;

	startIndex = glm::max(startIndex, 0u);
	endIndex = glm::min(endIndex, scene->GetTriangles().size());
	bbox = AABB(scene->GetTriangles().cbegin() + startIndex, scene->GetTriangles().cbegin() + endIndex, scene);
	if (endIndex - startIndex > trianglesInLeaf) {
		BuildTree();
	} else {
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
		scene->SortTriangles(startIndex, endIndex, axis);
		AABB totalBoundingBox(scene->GetTriangles().cbegin() + startIndex, scene->GetTriangles().cbegin() + endIndex, scene);
		float* surfaceAreas = new float[length];
		float* reverseSurfaceAreas = new float[length];
		AABB boundingBox;
		AABB reverseBoundingBox;
		for (auto splittingPlane = 0u; splittingPlane < length; ++splittingPlane) {
			unsigned int triangleIndex = startIndex + splittingPlane;
			boundingBox.Add(scene->GetTriangles()[triangleIndex], scene);
			surfaceAreas[splittingPlane] = boundingBox.GetSurfaceArea();

			unsigned int reverseTriangleIndex = endIndex - 1 - splittingPlane;
			reverseBoundingBox.Add(scene->GetTriangles()[reverseTriangleIndex], scene);
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

	scene->SortTriangles(startIndex, endIndex, minAxis);
	this->leftNode = std::unique_ptr<BVH>(new BVH(scene, startIndex, minIndex));
	this->rightNode = std::unique_ptr<BVH>(new BVH(scene, minIndex, endIndex));
}

bool BVH::IsLeafNode() const
{
	return isLeafNode;
}

RaycastResult BVH::Intersect(const Ray& ray) const
{
	RaycastResult currentBBoxResult = this->bbox.Intersect(ray);
	if (!currentBBoxResult.hit)
		return RaycastResult();

	if (this->IsLeafNode()) {
		return IntersectTriangles(ray);
	}

	RaycastResult leftBBoxResult = leftNode->GetAABB().Intersect(ray);
	RaycastResult rightBBoxResult = rightNode->GetAABB().Intersect(ray);

	if (!leftBBoxResult.hit && !rightBBoxResult.hit)
		return RaycastResult();

	if (leftBBoxResult.hit && !rightBBoxResult.hit) {
		return leftNode->Intersect(ray);
	} else if (!leftBBoxResult.hit && rightBBoxResult.hit) {
		return rightNode->Intersect(ray);
	} else {
		// Both child bounding boxes hit
		BVH* nearChild = leftNode.get();
		RaycastResult* nearBBoxResult = &leftBBoxResult;
		BVH* farChild = rightNode.get();
		RaycastResult* farBBoxResult = &rightBBoxResult;
		if (leftBBoxResult.distance > rightBBoxResult.distance) {
			std::swap(nearChild, farChild);
			std::swap(nearBBoxResult, farBBoxResult);
		}

		RaycastResult nearResult = nearChild->Intersect(ray);
		if (nearResult.hit) {
			if (nearResult.distance < farBBoxResult->distance)
				return nearResult;

			RaycastResult farResult = farChild->Intersect(ray);
			if (nearResult.distance < farResult.distance)
				return nearResult;
			else
				return farResult;
		} else {
			return farChild->Intersect(ray);
		}
	}
}

RaycastResult BVH::IntersectTriangles(const Ray& ray) const {
	RaycastResult result;
	for (auto triangleIter = scene->GetTriangles().cbegin() + startIndex;
		triangleIter < scene->GetTriangles().cbegin() + endIndex;
		++triangleIter) {
		glm::vec3 v0 = scene->GetVertices()[triangleIter->x].position;
		glm::vec3 v1 = scene->GetVertices()[triangleIter->y].position;
		glm::vec3 v2 = scene->GetVertices()[triangleIter->z].position;
		glm::vec3 currentIntersectionResult;
		bool hit = glm::intersectLineTriangle(ray.origin, ray.direction, v0, v1, v2, currentIntersectionResult);
		if (hit) {
			float candidateDistance = currentIntersectionResult.x;
			if (candidateDistance < result.distance && candidateDistance > 0.0f) {
				result.distance = candidateDistance;
				result.position = ray.origin + currentIntersectionResult.x * ray.direction;
				result.barycentric = glm::vec3(currentIntersectionResult.y,
					currentIntersectionResult.z,
					1.0f - currentIntersectionResult.y - currentIntersectionResult.z);
				/* HACK: Uses material from one vertex.
				Materials should in any case be triangle-specific, not vertex-specific. */
				result.materialIndex = scene->GetVertices()[triangleIter->x].materialIndex;
				result.hit = true;
			}
		}
	}
	return result;
}

const AABB& BVH::GetAABB() const
{
	return bbox;
}
