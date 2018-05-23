#include "fbxModel.h"

using namespace std;

void Mesh::setupMesh(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices, const std::vector<VertexBoneData>& bones)  // 建立VAO,VBO等缓冲区
{
	glGenVertexArrays(1, &this->VAOId);
	glGenBuffers(1, &this->vertexVBO);
	glGenBuffers(1, &this->EBOId);
	if (isAnim)
		glGenBuffers(1, &this->boneVBO);

	glBindVertexArray(this->VAOId);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertData.size(), &vertData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GL_FLOAT)));
	
	if (isAnim)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->boneVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)(4 * sizeof(GL_INT)));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBOId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::draw() const
{
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0 || (boneVBO == 0 && isAnim))
		return;

	glBindVertexArray(this->VAOId);
	int diffuseCnt = 0, specularCnt = 0, texUnitCnt = 1;
	for (auto it = this->textures.begin(); this->textures.end() != it; ++it)
	{
		switch ((*it)->getType())
		{
		case Texture::TEXTURE_DIFFUSE:
		{
			/*glActiveTexture(GL_TEXTURE0 + texUnitCnt);
			glBindTexture(GL_TEXTURE_2D, (*it)->getID());
			std::stringstream samplerNameStr;
			samplerNameStr << "Texture_D" << diffuseCnt++;
			glUniform1i(glGetUniformLocation(shader.getID(),
				samplerNameStr.str().c_str()), texUnitCnt++);*/

			(*it)->bind(Texture::TEXTURE_DIFFUSE);
		}
		break;
		case Texture::TEXTURE_SPECULAR:
		{
			/*glActiveTexture(GL_TEXTURE0 + texUnitCnt);
			glBindTexture(GL_TEXTURE_2D, (*it)->getID());
			std::stringstream samplerNameStr;
			samplerNameStr << "Texture_S" << specularCnt++;
			glUniform1i(glGetUniformLocation(shader.getID(),
				samplerNameStr.str().c_str()), texUnitCnt++);*/
		}
		break;
		default:
			std::cerr << "Warning::Mesh::draw, texture type" << (*it)->getType()
				<< " current not supported." << std::endl;
			break;
		}
	}
	
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setData(const std::vector<Vertex>& vertData,
	const std::vector<Texture*>& textures,
	const std::vector<GLuint>& indices,
	const std::vector<VertexBoneData>& bones)
{
	this->textures = textures;
	size = indices.size();
	if (!vertData.empty() && !indices.empty() && (!bones.empty() || !isAnim))
	{
		this->setupMesh(vertData, indices, bones);
	}
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAOId);
	glDeleteBuffers(1, &this->vertexVBO);
	glDeleteBuffers(1, &this->EBOId);
	glDeleteBuffers(1, &this->boneVBO);
}

void FbxModel::draw() const
{
	for (const Mesh* mesh : meshes)
		mesh->draw();
}

void FbxModel::drawDepthMap() const
{
	for (const Mesh* mesh : meshes)
		mesh->draw();
}

bool FbxModel::loadModel(const std::string& filePath)
{
	if (filePath.empty())
	{
		std::cerr << "Error:AnimModel::loadAnimModel, empty AnimModel file path." << std::endl;
		return false;
	}
	//sceneObjPtr = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	sceneObjPtr = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!sceneObjPtr)
	{
		std::cerr << "Error:AnimModel::loadAnimModel, description: " << importer.GetErrorString() << std::endl;
		return false;
	}
	this->modelFileDir = filePath.substr(0, filePath.find_last_of('/'));
	
	m_GlobalInverseTransform = convertMat(sceneObjPtr->mRootNode->mTransformation);
	m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);

	if (!this->processNode(sceneObjPtr->mRootNode, sceneObjPtr))
	{
		std::cerr << "Error:AnimModel::loadAnimModel, process node failed." << std::endl;
		return false;
	}

	pos_info = glm::vec3((xmax + xmin) / 2, (ymax + ymin) / 2, (ymax + ymin) / 2);
	len_info = glm::vec3((xmax - xmin) / 2, (ymax - ymin) / 2, (ymax - ymin) / 2);

	return true;
}

bool FbxModel::processNode(const aiNode* node, const aiScene* sceneObjPtr)
{
	if (!node || !sceneObjPtr)
		return false;

	for (size_t i = 0; i < node->mNumMeshes; ++i)
	{
		const aiMesh* meshPtr = sceneObjPtr->mMeshes[node->mMeshes[i]];
		if (meshPtr)
		{
			Mesh* meshObj = new Mesh(isAnim);
			if (this->processMesh(meshPtr, sceneObjPtr, *meshObj))
			{
				this->meshes.push_back(meshObj);
			}
		}
	}

	for (size_t i = 0; i < node->mNumChildren; ++i)
	{
		this->processNode(node->mChildren[i], sceneObjPtr);
	}

	return true;
}

bool FbxModel::processMesh(const aiMesh* meshPtr, const aiScene* sceneObjPtr, Mesh& meshObj)
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

bool FbxModel::processMaterial(const aiMaterial* matPtr, const aiScene* sceneObjPtr,
	const aiTextureType textureType, std::vector<Texture*>& textures)
{
	textures.clear();

	if (!matPtr || !sceneObjPtr)
		return false;
	if (matPtr->GetTextureCount(textureType) <= 0)
		return true;
	for (size_t i = 0; i < matPtr->GetTextureCount(textureType); ++i)
	{
		aiString textPath;
		aiReturn retStatus = matPtr->GetTexture(textureType, i, &textPath);
		if (retStatus != aiReturn_SUCCESS
			|| textPath.length == 0)
		{
			std::cerr << "Warning, load texture type=" << textureType
				<< "index= " << i << " failed with return value= "
				<< retStatus << std::endl;
			continue;
		}
		std::string path = textPath.C_Str();
		std::string absolutePath = "Resource/tga/" + path.substr(path.find_last_of('\\') + 1);//this->modelFileDir + "/" + textPath.C_Str();
		if (loadedTextureMap.find(absolutePath) == loadedTextureMap.end())
		{
			cout << absolutePath << endl;
			Texture* text = new Texture();
			text->load2DTexture(absolutePath.c_str());
			//text->setType(Texture::TextureType(textureType));
			textures.push_back(text);
			loadedTextureMap[absolutePath] = text;
		}
		else
		{
			textures.push_back(loadedTextureMap[absolutePath]);
		}
	}
	return true;
}

glm::mat4 FbxModel::convertMat(const aiMatrix4x4& mat)
{
	return glm::mat4(mat[0][0], mat[1][0], mat[2][0], mat[3][0],
		mat[0][1], mat[1][1], mat[2][1], mat[3][1],
		mat[0][2], mat[1][2], mat[2][2], mat[3][2],
		mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
}

glm::mat4 FbxModel::convertMat(const aiMatrix3x3& mat)
{
	return glm::mat4(mat[0][0], mat[1][0], mat[2][0], 0.0,
		mat[0][1], mat[1][1], mat[2][1], 0.0,
		mat[0][2], mat[1][2], mat[2][2], 0.0,
		0.0, 0.0, 0.0, 1.0);
}
