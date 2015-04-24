#include "SceneFactory.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.h"
#include "Material.h"

std::shared_ptr<Scene> SceneFactory::CreateFromFile(const std::string& filename) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_ImproveCacheLocality |
		aiProcess_JoinIdenticalVertices |
		aiProcess_PreTransformVertices);

	if (scene == NULL) {
		std::printf("\nImport failed:\n\t");
		auto errorString = importer.GetErrorString();
		std::printf(errorString);
		std::printf("\n");
		return nullptr;
	}
	Scene::vertexList vertices = GetVertices(scene);
	Scene::triangleList faces = GetFaces(scene);
	Scene::materialList materials = GetMaterials(scene);
	return std::shared_ptr<Scene>(new Scene(vertices, faces, materials));
}

Scene::materialList SceneFactory::GetMaterials(const aiScene* scene) {
	if (!scene->HasMaterials())
		return std::vector<Material>();
	std::vector<Material> materials;
	materials.reserve(scene->mNumMaterials);
	for (auto materialIndex = 0u; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* assimpMaterial = scene->mMaterials[materialIndex];
		aiColor3D assimpDiffuseColor;
		assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, assimpDiffuseColor);
		Material material(glm::vec3(assimpDiffuseColor.r, assimpDiffuseColor.g, assimpDiffuseColor.b));
		materials.push_back(material);
	}
	return materials;
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
			aiVector3D assimpNormal = assimpMesh->mNormals[vertexIndex];
			Vertex vertex;
			vertex.position = glm::vec3(assimpVertex.x, assimpVertex.y, assimpVertex.z);
			vertex.normal = glm::vec3(assimpNormal.x, assimpNormal.y, assimpNormal.z);
			vertex.materialIndex = assimpMesh->mMaterialIndex;
			vertices.push_back(vertex);
		}
	}

	return vertices;
}

Scene::triangleList SceneFactory::GetFaces(const aiScene* scene) {
	int numTriangles = 0;
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		struct aiMesh* assimpMesh = scene->mMeshes[meshIndex];
		numTriangles += assimpMesh->mNumFaces;
	}

	Scene::triangleList triangles;
	triangles.reserve(numTriangles);
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		struct aiMesh* assimpMesh = scene->mMeshes[meshIndex];
		int meshStartIndex = GetHighestIndexIn(triangles);
		for (unsigned int faceIndex = 0; faceIndex < assimpMesh->mNumFaces; ++faceIndex) {
			struct aiFace assimpFace = assimpMesh->mFaces[faceIndex];
			glm::ivec3 face(assimpFace.mIndices[0], assimpFace.mIndices[1], assimpFace.mIndices[2]);
			if (meshStartIndex > 0) {
				face += meshStartIndex + 1;
			}
			triangles.push_back(face);
		}
	}

	return triangles;
}

int SceneFactory::GetHighestIndexIn(const Scene::triangleList& list) {
	int highestIndex = 0;
	for (auto face : list) {
		for (int i = 0; i < 3; ++i) {
			highestIndex = glm::max(highestIndex, face[i]);
		}
	}
	return highestIndex;
}