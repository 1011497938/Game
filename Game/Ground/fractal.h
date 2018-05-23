#pragma once  

using namespace std;
#include <vector>

class fractal
{
private:
	enum { MAX = 10000 };
	float roughness;
	
	int deltaHeight;
	
	int D;
public:
	vector<vector<float>> data;
	float a, b, c, d;
	int times;
	float step;
	int n;

	void calculate();
	fractal(int _times, int _step);
	~fractal();
	void seting(float _a, float _b, float _c, float _d, int  _deltaHeight, int _D, int seed);
};