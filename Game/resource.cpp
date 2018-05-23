#include "resource.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <unordered_map>
#include "texture.h"

void SubObj::setData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices, Texture* tex) {
	this->tex = tex;
	for (const Vertex& vertex : vertData) {
		if (vertex.position.x > xmax) xmax = vertex.position.x;
		if (vertex.position.x < xmin) xmin = vertex.position.x;
		if (vertex.position.y > ymax) ymax = vertex.position.y;
		if (vertex.position.y < ymin) ymin = vertex.position.y;
		if (vertex.position.z > zmax) zmax = vertex.position.z;
		if (vertex.position.z < zmin) zmin = vertex.position.z;
	}
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

void SubObj::draw() const
{
	tex->bind(Texture::TEXTURE_DIFFUSE);
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


int ResourceManager::BMP_Header_Length = 54;

GLubyte* ResourceManager::loadBMP(const string& fileName, GLuint& width, GLuint& height)
{
	GLint total_bytes;
	GLubyte* pixels = NULL;

	ifstream fin;
	fin.open(fileName, ios::binary);
	if (!fin.is_open())
		throw Error();
	fin.seekg(0x12, ios::beg);
	fin.read(reinterpret_cast<char*>(&width), 4);
	fin.read(reinterpret_cast<char*>(&height), 4);
	fin.seekg(BMP_Header_Length, ios::beg);
	GLint line_bytes = width * 3;
	while (line_bytes % 4 != 0)
		++line_bytes;
	total_bytes = line_bytes * height;
	pixels = new GLubyte[total_bytes];
	if (pixels == NULL)
	{
		fin.close();
		throw Error();
	}
	fin.read(reinterpret_cast<char*>(pixels), total_bytes);
	if (fin.eof())
	{
		delete pixels;
		fin.close();
		throw Error();
	}
	fin.close();

	return pixels;
}

Image* ResourceManager::loadImage(const string& fileName)
{
	CImage img;
	HRESULT hr = img.Load(fileName.c_str());
	if (!SUCCEEDED(hr))
		throw Error("ÎÄ¼þ¼ÓÔØÊ§°Ü");
	HBITMAP hbmp = img;
	BITMAP bm;
	if (!GetObject(hbmp, sizeof(bm), &bm))
		throw Error();

	int size = bm.bmHeight*bm.bmWidth;
	Image* r = new Image();
	r->height = bm.bmHeight;
	r->width = bm.bmWidth;
	r->data = new GLubyte[size*3];
	memcpy_s(r->data, size*3, bm.bmBits, size*3);

	return r;
}

void ResourceManager::writeOBJ(const string& fileName, const std::vector<Vertex>& outData, const std::vector<GLuint>& outIndices)
{
	ofstream fout;
	fout.open(fileName);
	if (!fout.is_open())
		throw Error("Can't open the file !");

	stringstream ss;
	fout.precision(8);
	fout.flags(ios::left | ios::fixed);
	fout.fill('0');
	
	for (Vertex v : outData) {
		fout << "v "; 
		fout.width(6); 
		fout << v.position.x / 100 << " "; 
		fout.width(6); 
		fout << v.position.y / 100 << " ";
		fout.width(6); 
		fout << v.position.z / 100 << "\n";
	}
	for (Vertex v : outData) {
		fout << "vn ";
		fout.width(6); 
		fout << v.normal.x << " ";
		fout.width(6);
		fout << v.normal.y << " ";
		fout.width(6); 
		fout << v.normal.z << "\n";
	}
	for (int i = 0; i < outIndices.size(); i += 3) {
		fout << "f " << outIndices[i] + 1 << "//" << outIndices[i] + 1 << " "
			<< outIndices[i + 1] + 1 << "//" << outIndices[i + 1] + 1 << " "
			<< outIndices[i + 2] + 1 << "//" << outIndices[i + 2] + 1 << "\n";
	}
	fout.close();
}

void ResourceManager::loadOBJ(const string& fileName, std::vector<SubObj*>& objs)
{
	ifstream fin;
	fin.open(fileName);
	if (!fin.is_open())
		throw Error("Can't open the file !");
	char lineBytes[300];

	while (!fin.eof())
	{
		fin.getline(lineBytes, 300);
		if (lineBytes[0] == 'm')
			break;
	}
	stringstream s0(lineBytes);
	string file, texName;
	s0 >> file;
	s0 >> file;
	ifstream texFile;
	texFile.open("Resource\\model\\"+file);
	map<string, string> texMap;
	while (!texFile.eof())
	{
		texFile.getline(lineBytes, 300);
		if (lineBytes[0] == 'n')
		{
			stringstream s1(lineBytes);
			s1 >> texName;
			s1 >> texName;
			texFile.getline(lineBytes, 300);
			stringstream s2(lineBytes);
			s2 >> file;
			texMap[texName] = file;
		}
	}

	vector<Vertex> inData;
	vector<GLuint> inIndices;
	vector<glm::vec3*> pos, normal;
	vector<glm::vec3*> texcoord;
	vector<glm::vec3*> index;
	int pCnt, nCnt, tCnt, iCnt;
	pCnt = nCnt = tCnt = iCnt = 0;

	string word;
	int flag = 0;
	Texture *tex = NULL;
	while (!fin.eof())
	{
		fin.getline(lineBytes, 300);
		if (lineBytes[0] == 'u')
		{
			flag = 1;
			stringstream ss(lineBytes);
			ss >> word;
			ss >> word;
			tex = new Texture();
			cout << texMap[word] << endl;
			tex->load2DTexture(("Resource\\model\\" + texMap[word]).c_str());
		}
		if (lineBytes[0] == 'f' && flag == 1)
			flag = 2;
		if (!(lineBytes[0] == 'v' || lineBytes[0] == 'f'))
		{
			if (flag == 2) {
				flag = 0;
				unordered_map<int, int> posi;
				int count = 0;
				inIndices.resize(index.size()-iCnt);
				for (int i = iCnt; i < index.size(); i++) {
					int k = index[i]->x - 1;
					if (posi.find(k) == posi.end()) {
						posi[k] = count;
						inData.push_back(Vertex());
						inData[count].position = *pos[index[i]->x - 1];
						inData[count].texCoords = glm::vec2(*texcoord[index[i]->y - 1]);
						inData[count].normal = *normal[index[i]->z - 1];
						inIndices[i - iCnt] = count;
						count++;
					}
					else {
						inIndices[i - iCnt] = posi[k];
					}			
				}
				SubObj* o = new SubObj();
				o->setData(inData, inIndices, tex);
				objs.push_back(o);
				iCnt = index.size();
				inData.resize(0);
				inIndices.resize(0);
			}
			continue;
		}
		stringstream ss(lineBytes);
		ss >> word;
		if (word.compare("v") == 0) {
			glm::vec3* vec = new glm::vec3(0.0f);
			ss >> vec->x >> vec->y >> vec->z;
			pos.push_back(vec);
		}
		else if (word.compare("vt") == 0) {
			glm::vec3* vec = new glm::vec3(0.0f);
			ss >> vec->x >> vec->y >> vec->z;
			texcoord.push_back(vec);
		}
		else if (word.compare("vn") == 0) {
			glm::vec3* vec = new glm::vec3(0.0f);
			ss >> vec->x >> vec->y >> vec->z;
			normal.push_back(vec);
		}
		else if (word.compare("f") == 0) {
			for (int i = 0; i < 3; i++) {
				glm::vec3* vec = new glm::vec3(0.0f);
				ss >> vec->x;
				ss.get();
				ss >> vec->y;
				ss.get();
				ss >> vec->z;
				index.push_back(vec);
			}
		}
	}
	fin.close();	
}