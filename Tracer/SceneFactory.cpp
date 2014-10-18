#include "SceneFactory.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <vector>

Scene SceneFactory::CreateFromFile(const std::string& filename) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_ImproveCacheLocality |
		aiProcess_FindDegenerates |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_PreTransformVertices);

	Scene::vertexList vertices = GetVertices(scene);
	Scene::faceList faces = GetFaces(scene);
	return Scene(vertices, faces);
}

Scene::vertexList SceneFactory::GetVertices(const aiScene* scene) {
	int numVertices = 0;
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		struct aiMesh* assimpMesh = scene->mMeshes[meshIndex];
		numVertices += assimpMesh->mNumVertices;
	}

	Scene::vertexList vertices;
	vertices.reserve(numVertices);
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		struct aiMesh* assimpMesh = scene->mMeshes[meshIndex];
		for (unsigned int vertexIndex = 0; vertexIndex < assimpMesh->mNumVertices; ++vertexIndex) {
			aiVector3D assimpVertex = assimpMesh->mVertices[vertexIndex];
			glm::vec3 vertex(assimpVertex.x, assimpVertex.y, assimpVertex.z);
			vertices.push_back(vertex);
		}
	}

	return vertices;
}

Scene::faceList SceneFactory::GetFaces(const aiScene* scene) {
	int numFaces = 0;
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		struct aiMesh* assimpMesh = scene->mMeshes[meshIndex];
		numFaces += assimpMesh->mNumFaces;
	}

	Scene::faceList faces;
	faces.reserve(numFaces);
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		struct aiMesh* assimpMesh = scene->mMeshes[meshIndex];
		int meshStartIndex = GetHighestIndexIn(faces);
		for (unsigned int faceIndex = 0; faceIndex < assimpMesh->mNumFaces; ++faceIndex) {
			struct aiFace assimpFace = assimpMesh->mFaces[faceIndex];
			glm::ivec3 face(assimpFace.mIndices[0], assimpFace.mIndices[1], assimpFace.mIndices[2]);
			if (meshStartIndex > 0) {
				face += meshStartIndex + 1;
			}
			faces.push_back(face);
		}
	}

	return faces;
}

int SceneFactory::GetHighestIndexIn(const Scene::faceList& list) {
	int highestIndex = 0;
	for (auto face : list) {
		for (int i = 0; i < 3; ++i) {
			highestIndex = glm::max(highestIndex, face[i]);
		}
	}
	return highestIndex;
}