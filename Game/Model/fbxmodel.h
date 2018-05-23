#pragma once

#include "model.h"

#include <vector>
#include <map>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>           
#include <iostream>
#include <sstream>

typedef unsigned uint;
class Texture;
class Shader;

struct VertexBoneData
{
	unsigned int IDs[4] = { 0, 0, 0, 0 };
	float Weights[4] = { 0.0, 0.0, 0.0, 0.0 };
	void AddBoneData(uint BoneID, float Weight)
	{
		for (uint i = 0; i < 4; i++) {
			if (Weights[i] == 0.0) {
				IDs[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}
	}
};

class Mesh
{
public:
	void draw() const;
	Mesh(const int& isAnim) :VAOId(0), EBOId(0), vertexVBO(0), boneVBO(0), isAnim(isAnim) {}
	~Mesh();
	void setData(const std::vector<Vertex>& vertData,
		const std::vector<Texture*>& textures,
		const std::vector<GLuint>& indices,
		const std::vector<VertexBoneData>& bones);

private:
	std::vector<Texture*> textures;
	GLuint VAOId, EBOId, boneVBO, vertexVBO;
	int size, isAnim;
	void setupMesh(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices, const std::vector<VertexBoneData>& bones);
};

class FbxModel :public Model
{
public:
	FbxModel()
	{
		xmin = ymin = zmin = 10000;
		xmax = ymax = zmax = -10000;
	}
	~FbxModel()
	{
		for (const Mesh* mesh : meshes)
			delete mesh;
		for (auto t : loadedTextureMap)
			delete t.second;
	}
	bool loadModel(const std::string& filePath);
	void draw() const;
	void drawDepthMap() const;

protected:
	bool processNode(const aiNode* node, const aiScene* sceneObjPtr);
	virtual bool processMesh(const aiMesh* meshPtr, const aiScene* sceneObjPtr, Mesh& meshObj);
	bool processMaterial(const aiMaterial* matPtr, const aiScene* sceneObjPtr,
		const aiTextureType textureType, std::vector<Texture*>& textures);
	glm::mat4 convertMat(const aiMatrix4x4& mat);
	glm::mat4 convertMat(const aiMatrix3x3& mat);

protected:
	Assimp::Importer importer;
	const aiScene* sceneObjPtr;
	std::vector<Mesh*> meshes;
	glm::mat4 m_GlobalInverseTransform;
	std::map<std::string, Texture*> loadedTextureMap;
	int isAnim = 0;
	float xmin, xmax, ymin, ymax, zmin, zmax;
};