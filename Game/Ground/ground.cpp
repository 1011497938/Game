#include "ground.h"
#include <fstream>
#include <iostream>
#include "../renderer.h"
#include "../shadow.h"
#include <vector>
#include <time.h>    
#include <math.h>    
#include "fractal.h"

//测试
#include "../Model/baseModel.h"
#include <sstream> 

using namespace glm;
using namespace std;

inline double magicAbs(double num, int times) {

	for (int i = 0; i < times; i++)
		num = abs(num - 0.5) * 2;

	return num;
}

Ground::Ground(string _filePath, const  int &_fileN, const float amplify, const int _width) {
	times = 3;//3
	D = 10;

	timeCounter = time(0);

	width = _width;
	int fileN = _fileN;
	areaNum = fileN - 1;

	//读取文件
	definedH = new int[fileN * fileN];

	fstream mapFile(_filePath);
	if (!mapFile) {
		cout << "Unable to open map file";
		while (true);
	}

	long long int total = 0;
	for (int i = 0; i < fileN * fileN; i++)
	{
		mapFile >> definedH[i];
		total += definedH[i];
	}

	freactalN = pow(2, times) + 1;
	n = freactalN * areaNum - areaNum + 1;
	step = width*1.0 / n;
	cout << "step:" << step << endl;
	total /= fileN * fileN;
	deltaH = 3;

	for (int i = 0; i < fileN * fileN; i++) {
		definedH[i] = (definedH[i] - total) * amplify;
	}
	mapFile.close();
	data.resize(n);
	for (int i = 0; i < n; i++) {
		data[i].resize(n);
		for (int j = 0; j < n; j++) {
			data[i][j] = 0;
		}
	}

	//计算高度
	calculate();
	delete definedH;

	float bar_x = 179 / step;
	float bar_y = 378 / step;
	int bar_height = data[bar_x][bar_y];
	int bar_width = 50;
	int bar_point = bar_width / step / 2;
	cout << bar_height << endl;

	minHeight = 10000;
	maxHeight = -10000;
	totalV.resize(n);

	for (int i = 0; i < n; i++) {
		totalV[i].resize(n);
		for (int j = 0; j < n; j++) {

			if (i <  bar_x + bar_point &&  i >  bar_x - bar_point  &&  j <  bar_y + bar_point &&  j >  bar_y - bar_point) {
				float k = magicAbs((i - (bar_x - bar_point)) / bar_point / 2, 1);
				data[i][j] = bar_height * (1-k*k) + data[i][j] * k*k;
			}


			totalV[i][j].position.x = i * step;
			totalV[i][j].position.y = j * step;
			totalV[i][j].position.z = data[i][j];

			if (i != 0 && j != 0 && i != n - 1 && j != n - 1)
				totalV[i][j].normal = -getNormal(i, j);
				//totalV[i][j].normal = vec3(0, 0, -1);
			else
				totalV[i][j].normal = vec3(0, 0, -1);
			
			

			if (i <  bar_x + bar_point &&  i >  bar_x - bar_point  &&  j <  bar_y + bar_point &&  j >  bar_y - bar_point) {
				totalV[i][j].texCoords.x = i*step;
				totalV[i][j].texCoords.y = j*step;
			}
			else if ( i <  bar_x + n/4 &&  i >  bar_x - n / 4 &&  j <  bar_y + n / 4 &&  j >  bar_y - n / 4) {
				totalV[i][j].texCoords.x = i*step/4;
				totalV[i][j].texCoords.y = j*step/4;
			}else if (i <  bar_x + n / 3 && i >  bar_x - n / 3 && j <  bar_y + n / 3 && j >  bar_y - n / 3) {
				totalV[i][j].texCoords.x = i*step / 16;
				totalV[i][j].texCoords.y = j*step / 16;
			}else{
				totalV[i][j].texCoords.x = 0.3;
				totalV[i][j].texCoords.y = 0.3;
			}



			if (minHeight > data[i][j])
				minHeight = data[i][j];
			if (maxHeight < data[i][j])
				maxHeight = data[i][j];
		}
	}
	cout << "地图计算完毕总共" << n*n << "个点, 费时" << time(0) - timeCounter << "s" << endl;
	cout << "最高点：" << maxHeight << "   最低点：" << minHeight << endl;

	loadModel("");
	loadTexture("Resource\\pic\\ground.jpg");
	cout << "地区加载完毕, 费时" << time(0) - timeCounter << "s" << endl;
}


Ground::~Ground() {
	//delete whole;
}




void  Ground::calculate() {
	srand(time(0));
	cout << "地图计算开始" << endl;

	fractal* divF = new fractal(times, step);
	vector<float> tempX, tempY;

	tempX.resize(freactalN * areaNum);
	tempY.resize(freactalN);


	for (int j = 0; j <areaNum; j++) {
		for (int i = 0; i < areaNum; i++) {
			int this_a = definedH[i + j * (areaNum + 1)];
			int this_b = definedH[i + 1 + j * (areaNum + 1)];
			int this_c = definedH[i + 1 + (j + 1) * (areaNum + 1)];
			int this_d = definedH[i + (j + 1) * (areaNum + 1)];
			//cout << i << " " << j << endl;
			//(float _a, float _b, float _c, float _d, int  _deltaHeight, int _D, int seed)
			divF->seting(this_a, this_b, this_c, this_d, deltaH, 2, rand());

			if (i != 0) {
				for (int j = 0; j <freactalN; j++)
					divF->data[0][j] = tempY[j];
			}
			if (j != 0) {
				for (int j = 0; j <freactalN; j++)
					divF->data[j][0] = tempX[j + freactalN * i];
			}

			divF->calculate();

			for (int k = 0; k <freactalN; k++) {
				tempX[k + freactalN * i] = divF->data[k][freactalN - 1];
				tempY[k] = divF->data[freactalN - 1][k];
			}

			merge(i, j, divF->data);
		}
	}

	cout << "生成高度点阵，费时:" << time(0) - timeCounter << "s" << endl;

	//for (int i = 0; i < n; i++)
	//	for (int j = 0; j < n; j++)
	//		if (i != 0 && i != n - 1 && j != 0 && j != n - 1)
	//nVector[i][j] = getNormal(i, j);
	//nVector[i][j] = vec3(0,0,1);

	//cout << "生成法向量，费时:" << time(0) - timeCounter << "s" << endl;

}


vec3 Ground::getNormal(int i, int j) {
	glm::vec3 a = glm::normalize(
		glm::vec3((i + 0)* step - n*step / 2, (j - 1)*step - n*step / 2, data[i + 0][j - 1]) - glm::vec3(i* step - n*step / 2, j*step - n*step / 2, data[i][j])
	);
	glm::vec3 b = glm::normalize(
		glm::vec3((i + 1)* step - n*step / 2, (j - 1)*step - n*step / 2, data[i + 1][j - 1]) - glm::vec3(i* step - n*step / 2, j*step - n*step / 2, data[i][j])
	);
	glm::vec3 c = glm::normalize(
		glm::vec3((i + 1)* step - n*step / 2, (j + 0)*step - n*step / 2, data[i + 1][j + 0]) - glm::vec3(i* step - n*step / 2, j*step - n*step / 2, data[i][j])
	);
	glm::vec3 e = glm::normalize(
		glm::vec3((i + 0)* step - n*step / 2, (j + 1)*step - n*step / 2, data[i + 0][j + 1]) - glm::vec3(i* step - n*step / 2, j*step - n*step / 2, data[i][j])
	);
	glm::vec3 f = glm::normalize(
		glm::vec3((i - 1)* step - n*step / 2, (j + 1)*step - n*step / 2, data[i - 1][j + 1]) - glm::vec3(i* step - n*step / 2, j*step - n*step / 2, data[i][j])
	);
	glm::vec3 g = glm::normalize(
		glm::vec3((i - 1)* step - n*step / 2, (j + 0)*step - n*step / 2, data[i - 1][j + 0]) - glm::vec3(i* step - n*step / 2, j*step - n*step / 2, data[i][j])
	);

	return	(
		glm::cross(a, b) +
		glm::cross(b, c) +
		glm::cross(c, e) +
		glm::cross(e, f) +
		glm::cross(f, g) +
		glm::cross(g, a)
		);
}

void Ground::merge(int x, int y, const vector<vector<float>> &Fdata) {
	//cout << freactalN;
	int offestI = freactalN * x - x;
	int offestJ = freactalN * y - y;
	for (int i = 0; i <freactalN; i++) {
		for (int j = 0; j <freactalN; j++) {
			data[offestI + i][offestJ + j] = Fdata[i][j];
			//cout << ":";
		}
	}

}

float  Ground::getHeight(int x, int y) {
	return data[x][y];
}
float Ground::getObjectHeight(float x, float y) {
	if (x < 0 || y<0 || x>width || y > width) {
		return 0;
	}
	double xf, yf, z;
	int x0, y0;

	x0 = (int)(x / step);
	y0 = (int)(y / step);
	xf = 1 - ((x / step) - x0);
	yf = 1 - ((y / step) - y0);
	z = (getHeight(x0, y0) * xf + getHeight(x0 + 1, y0) * (1 - xf)) * yf
		+ (getHeight(x0, y0 + 1) * xf + getHeight(x0 + 1, y0 + 1) * (1 - xf)) * (1 - yf);

	return z;
}

//Model
void Ground::setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices)
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

void Ground::draw() const
{

	textureLow.bind(Texture::TEXTURE_DIFFUSE);

	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	//glDrawArrays(GL_TRIANGLES, 0, n);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Ground::drawDepthMap() const
{
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

bool Ground::loadModel(const std::string& filePath) {
	int areaX = 0;
	int areaY = 0;

	int showSize = n;
	vArray.resize(showSize * showSize);

	int indiceNum = 6 * (showSize - 1)*(showSize - 1);
	indice.resize(indiceNum);

	int count = 0;

	for (int i = areaX; i < areaX + showSize; i++) {
		for (int j = areaY; j < areaY + showSize; j++) {
			vArray[count] = totalV[i][j];

			count++;
		}

	}

	genIndice(0, showSize);

	setVboData(vArray, indice);

	return true;
}

void Ground::genIndice(int start, int n)
{
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			indice[start + 3 * ((n - 1) * i + j)] = (n * i + j);
			indice[start + 3 * ((n - 1) * i + j) + 1] = (n * i + j + 1);
			indice[start + 3 * ((n - 1) * i + j) + 2] = (n * (i + 1) + j + 1);
		}
	}
	//cout << endl;
	int off = 3 * (n - 1)*(n - 1);
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {

			indice[start + off + 3 * ((n - 1) * i + j)] = (n * (i + 1) + j + 1);
			indice[start + off + 3 * ((n - 1) * i + j) + 1] = (n * (i + 1) + j);
			indice[start + off + 3 * ((n - 1)* i + j) + 2] = (n * i + j);

			//indice[start + off + 3 * ((n - 1) * i + j)] = (n * i + j);
			//indice[start + off + 3 * ((n - 1) * i + j) + 1] = (n * (i + 1) + j);
			//indice[start + off + 3 * ((n - 1)* i + j) + 2] = (n * (i + 1) + j + 1);
		}
	}
}

bool Ground::loadTexture(const std::string& filePath, const bool& isHigh)
{
	Texture* tmp;
	if (isHigh)
		tmp = &textureHigh;
	else
		tmp = &textureLow;
	tmp->load2DTexture(filePath.c_str());

	return true;
}