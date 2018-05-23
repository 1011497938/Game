#include "animModel.h"

using namespace std;

bool AnimModel::processMesh(const aiMesh* meshPtr, const aiScene* sceneObjPtr, Mesh& meshObj)
{
	if (!meshPtr || !sceneObjPtr)
	{
		return false;
	}
	std::vector<Vertex> vertData;
	std::vector<Texture*> textures;
	std::vector<GLuint> indices;
	std::vector<VertexBoneData> bones;
	vertData.resize(meshPtr->mNumVertices);
	bones.resize(meshPtr->mNumVertices);
	indices.resize(meshPtr->mNumFaces * 3);

	for (size_t i = 0; i < meshPtr->mNumVertices; ++i)
	{
		Vertex vertex;
		if (meshPtr->HasPositions())
		{
			vertex.position.x = meshPtr->mVertices[i].x;
			vertex.position.y = meshPtr->mVertices[i].y;
			vertex.position.z = meshPtr->mVertices[i].z;
			if (vertex.position.x > xmax) xmax = vertex.position.x;
			if (vertex.position.x < xmin) xmin = vertex.position.x;
			if (vertex.position.y > ymax) ymax = vertex.position.y;
			if (vertex.position.y < ymin) ymin = vertex.position.y;
			if (vertex.position.z > zmax) zmax = vertex.position.z;
			if (vertex.position.z < zmin) zmin = vertex.position.z;
		}
		if (meshPtr->HasTextureCoords(0))
		{
			vertex.texCoords.x = meshPtr->mTextureCoords[0][i].x;
			vertex.texCoords.y = meshPtr->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		if (meshPtr->HasNormals())
		{
			vertex.normal.x = meshPtr->mNormals[i].x;
			vertex.normal.y = meshPtr->mNormals[i].y;
			vertex.normal.z = meshPtr->mNormals[i].z;
		}
		vertData[i] = vertex;
	}

	for (size_t i = 0; i < meshPtr->mNumFaces; ++i)
	{
		aiFace face = meshPtr->mFaces[i];
		if (face.mNumIndices != 3)
		{
			std::cerr << "Error:AnimModel::processMesh, mesh not transformed to triangle mesh." << std::endl;
			return false;
		}
		for (size_t j = 0; j < face.mNumIndices; ++j)
		{
			indices[i * 3 + j] = face.mIndices[j];
		}
	}

	for (size_t i = 0; i < meshPtr->mNumBones; ++i)
	{
		aiBone* bone = meshPtr->mBones[i];

		uint BoneIndex = 0;
		string BoneName(meshPtr->mBones[i]->mName.data);
		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
			BoneIndex = m_NumBones;
			m_NumBones++;
			m_BoneOffset.push_back(convertMat(meshPtr->mBones[i]->mOffsetMatrix));
			m_BoneMapping[BoneName] = BoneIndex;
		}
		else {
			BoneIndex = m_BoneMapping[BoneName];
		}

		for (uint j = 0; j < meshPtr->mBones[i]->mNumWeights; j++) {
			uint VertexID = meshPtr->mBones[i]->mWeights[j].mVertexId;
			float Weight = meshPtr->mBones[i]->mWeights[j].mWeight;
			bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}

	if (meshPtr->mMaterialIndex >= 0)
	{
		const aiMaterial* materialPtr = sceneObjPtr->mMaterials[meshPtr->mMaterialIndex];

		std::vector<Texture*> diffuseTexture;
		this->processMaterial(materialPtr, sceneObjPtr, aiTextureType_DIFFUSE, diffuseTexture);
		textures.insert(textures.end(), diffuseTexture.begin(), diffuseTexture.end());

		std::vector<Texture*> specularTexture;
		this->processMaterial(materialPtr, sceneObjPtr, aiTextureType_SPECULAR, specularTexture);
		textures.insert(textures.end(), specularTexture.begin(), specularTexture.end());
	}

	meshObj.setData(vertData, textures, indices, bones);
	return true;
}

void AnimModel::setBaseUniform(const Shader& shader, const Camera& camera, const Object& object)
{
	Model::setBaseUniform(shader, camera, object);
	glUniform1i(glGetUniformLocation(shader.getID(), "isAnim"), 1);
	char *t = new char[10];
	for (int i = 0; i < m_BoneTransform.size(); i++) {
		_itoa_s(i, t, 10, 10);
		glUniformMatrix4fv(glGetUniformLocation(shader.getID(), (string("gBones[") + t + ']').c_str()), 1, GL_FALSE, glm::value_ptr(m_BoneTransform[i]));
	}
}

void AnimModel::setDepthUniform(const Shader& depthShader, const glm::mat4& depthVP, Object& object)
{
	Model::setDepthUniform(depthShader, depthVP, object);
	glUniform1i(glGetUniformLocation(depthShader.getID(), "isAnim"), 1);
	char *t = new char[10];
	for (int i = 0; i < m_BoneTransform.size(); i++) {
		_itoa_s(i, t, 10, 10);
		glUniformMatrix4fv(glGetUniformLocation(depthShader.getID(), (string("gBones[") + t + ']').c_str()), 1, GL_FALSE, glm::value_ptr(m_BoneTransform[i]));
	}
}

void AnimModel::draw() const
{
	for (const Mesh* mesh : meshes)
		mesh->draw();
}

void AnimModel::drawDepthMap() const
{
	for (const Mesh* mesh : meshes)
		mesh->draw();
}

void AnimModel::getBoneTransform(const float& RunningTime)
{
	m_BoneTransform.clear();
	BoneTransform(RunningTime);
}

void AnimModel::BoneTransform(float TimeInSeconds)
{
	glm::mat4 Identity = glm::mat4(1.0f);

	float TicksPerSecond = sceneObjPtr->mAnimations[0]->mTicksPerSecond != 0 ? sceneObjPtr->mAnimations[0]->mTicksPerSecond : 25.0f;
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, sceneObjPtr->mAnimations[0]->mDuration);
	m_BoneTransform.resize(m_NumBones);
	ReadNodeHeirarchy(AnimationTime, sceneObjPtr->mRootNode, Identity);
}

void AnimModel::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
{
	string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = sceneObjPtr->mAnimations[0];

	glm::mat4 NodeTransformation(convertMat(pNode->mTransformation));

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {  
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		glm::mat4 ScalingM;
		ScalingM = glm::scale(ScalingM, glm::vec3(Scaling.x, Scaling.y, Scaling.z));

		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		glm::mat4 RotationM = convertMat(RotationQ.GetMatrix());
 
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		glm::mat4 TranslationM;
		TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));

		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		uint BoneIndex = m_BoneMapping[NodeName];
		m_BoneTransform[BoneIndex] = m_GlobalInverseTransform * GlobalTransformation * m_BoneOffset[BoneIndex];
	}

	for (uint i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

const aiNodeAnim* AnimModel::FindNodeAnim(const aiAnimation* pAnimation, const string& NodeName)
{
	for (int i = 0; i < pAnimation->mNumChannels; i++)
	{
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}
	
	return NULL;
}

void AnimModel::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	uint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	uint NextRotationIndex = (RotationIndex + 1);
	if (NextRotationIndex < pNodeAnim->mNumRotationKeys)
	{
		float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}
	else
	{
		Out = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	}
}

void AnimModel::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	uint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	uint NextPositionIndex = (PositionIndex + 1);
	if (NextPositionIndex < pNodeAnim->mNumPositionKeys)
	{
		float DeltaTime = pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime;
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}
	else
	{
		Out = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	}
}

void AnimModel::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	uint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	uint NextScalingIndex = (ScalingIndex + 1);
	if (NextScalingIndex < pNodeAnim->mNumScalingKeys)
	{
		float DeltaTime = pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime;
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}
	else
	{
		Out = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	}
}

uint AnimModel::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (uint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}
	return pNodeAnim->mNumRotationKeys - 1;
}

uint AnimModel::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (uint i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	return pNodeAnim->mNumPositionKeys -1;
}

uint AnimModel::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (uint i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}
	return pNodeAnim->mNumScalingKeys - 1;
}
