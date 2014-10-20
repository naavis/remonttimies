#pragma once
#include "Scene.h"
#include <assimp/scene.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <memory>

class SceneFactory
{
public:
	static std::shared_ptr<Scene> CreateFromFile(const std::string& filename);
private:
	static Scene::vertexList GetVertices(const aiScene* scene);
	static Scene::faceList GetFaces(const aiScene* scene);
	static int GetHighestIndexIn(const Scene::faceList& list);
};