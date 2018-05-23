#pragma once

#include "fbxModel.h"

class AnimModel :public FbxModel
{
public:
	AnimModel(){ isAnim = 1; }

	void getBoneTransform(const float& RunningTime);
	void draw() const;
	void drawDepthMap() const;
	void setBaseUniform(const Shader& shader, const Camera& camera, const Object& object);
	void setDepthUniform(const Shader& depthShader, const glm::mat4& depthVP, Object& object);

private:
	bool processMesh(const aiMesh* meshPtr, const aiScene* sceneObjPtr, Mesh& meshObj);
	void BoneTransform(float TimeInSeconds);
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string& NodeName);

private:
	std::map<std::string, uint> m_BoneMapping;
	int m_NumBones = 0;
	std::vector<glm::mat4> m_BoneOffset;
	std::vector<glm::mat4> m_BoneTransform;
};
