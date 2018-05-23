#pragma once

#include <fstream>
#include <iostream>
#include "../renderer.h"
#include "../shadow.h"
#include <vector>

typedef byte kind;

//#include "grass.h"
using namespace glm;
using namespace std;
class Ground : public Model{
public:
	Ground(string _filePath, const  int &_fileN, const float amplify, const int _width);
	~Ground();
	vector<vector<float>> data;
	float getHeight(int x, int y);   //朋洋大佬用

	float getObjectHeight(float x, float y);  //谭思危自用
	float step;
//private:

	int n;
	int areaNum;

	int D;
	int width;
	int deltaH;

	float minHeight;
	float maxHeight;

	Object* wholeT;
	//Terria *whole;

	int* definedH;

	vector <vector<Vertex>> totalV;

	void setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices);
	bool loadModel(const std::string& filePath);
	void draw() const;
	void drawDepthMap() const;
	bool loadTexture(const std::string& filePath, const bool& isHigh = false);

private:
	//非重要参数
	int freactalN;
	void Ground::merge(int x, int y, const vector<vector<float>> &Fdata);
	int timeCounter;

	int times;
	void  calculate();

	//Model
	GLuint VAOId, EBOId, vertexVBO;
	void genIndice(int start, int n);
	vector<Vertex> vArray;
	Texture textureHigh, textureLow;
	int size;
	std::vector<GLuint> indice;
	vec3 getNormal(int i, int j);
};

