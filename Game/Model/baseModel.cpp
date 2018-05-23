#include "baseModel.h"


void BaseModel::draw() const
{
	tex->bind(Texture::TEXTURE_DIFFUSE);
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BaseModel::drawDepthMap() const
{
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool BaseModel::loadModel(const std::string& modelType)
{
	tex = new Texture();
	tex->load2DTexture("Resource\\tga\\white.bmp");
	pos_info = glm::vec3(0.0f);
	stringstream s;
	s << modelType;
	string sub;
	s >> sub;
	if (sub.compare("Cube") == 0)
	{
		return loadCube(s);
	}
	else if (sub.compare("") == 0)
	{

	}
	
	return false;
}

void BaseModel::setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices)
{
	size = indices.size();
	glGenVertexArrays(1, &this->VAOId);
	glGenBuffers(1, &this->vertexVBO);
	glGenBuffers(1, &this->EBOId);

	glBindVertexArray(this->VAOId);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertData.size(), &vertData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GL_FLOAT)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBOId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

bool BaseModel::loadCube(stringstream& s)
{
	float l = 0, w = 0, h = 0;
	s >> l;
	s >> w;
	s >> h;

	if (l == 0 || w == 0 || h == 0)
		return false;

	vector<Vertex> vertex;
	vector<GLuint> indices;
	vertex.resize(24);
	indices.resize(36);
	len_info = glm::vec3(l / 2, w / 2, h / 2);

	for (int i = 0; i < 8; i++)
		vertex[i].texCoords = glm::vec2(0.5f);

	vector<glm::vec3> normal;
	normal.resize(6);

	{
		vertex[0].position = glm::vec3(-l / 2, -w / 2, h / 2);
		vertex[1].position = glm::vec3(-l / 2, w / 2, h / 2);
		vertex[2].position = glm::vec3(l / 2, w / 2, h / 2);
		vertex[3].position = glm::vec3(l / 2, -w / 2, h / 2);

		vertex[7].position = glm::vec3(-l / 2, -w / 2, -h / 2);
		vertex[6].position = glm::vec3(-l / 2, w / 2, -h / 2);
		vertex[5].position = glm::vec3(l / 2, w / 2, -h / 2);
		vertex[4].position = glm::vec3(l / 2, -w / 2, -h / 2);

		vertex[8].position = vertex[2].position;
		vertex[9].position = vertex[1].position;
		vertex[10].position = vertex[6].position;
		vertex[11].position = vertex[5].position;

		vertex[12].position = vertex[3].position;
		vertex[13].position = vertex[2].position;
		vertex[14].position = vertex[5].position;
		vertex[15].position = vertex[4].position;

		vertex[16].position = vertex[0].position;
		vertex[17].position = vertex[3].position;
		vertex[18].position = vertex[4].position;
		vertex[19].position = vertex[7].position;

		vertex[20].position = vertex[1].position;
		vertex[21].position = vertex[0].position;
		vertex[22].position = vertex[7].position;
		vertex[23].position = vertex[6].position;

		normal[0] = glm::vec3(0.0f, 0.0f, 1.0f);
		normal[1] = glm::vec3(0.0f, 0.0f, -1.0f);
		normal[2] = glm::vec3(0.0f, 1.0f, 0.0f);
		normal[3] = glm::vec3(1.0f, 0.0f, 0.0f);
		normal[4] = glm::vec3(0.0f, -1.0f, 0.0f);
		normal[5] = glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < 6; i++)
	{
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 0;
		indices[i * 6 + 4] = i * 4 + 2;
		indices[i * 6 + 5] = i * 4 + 3;
		for (int j = 0; j < 4; j++) {
			vertex[i * 4 + j].normal = normal[i];
			vertex[i * 4 + j].texCoords = glm::vec2(vertex[i * 4 + j].position.x, vertex[i * 4 + j].position.y);
		}
	}

	setVboData(vertex, indices);
	return true;
}