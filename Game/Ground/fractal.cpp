#include "fractal.h"    
#include <time.h>    
#include <math.h>    
#include <stdlib.h>    
#include <iostream>  
#include "gl/glut.h"  

// deltaHeight : ֵԽ�󣬵������Խ��   
// times  : ��������������Խ�࣬tireԽ��  
// step   : ����ʱһ������ĳ̶�  
// D      : �����˥�����ӣ�Խ��������������ȡֵԽС  
// seed   : ���������  

//int fractal::times = 4;
//float fractal::step = 10;
//int fractal::n = pow(2, fractal::times) + 1;

fractal::fractal(int _times, int _step)
{
	times = _times;
	n = pow(2, times) + 1;
	step = _step;
	data.resize(n);
	for (int i = 0; i < n; i++) {
		data[i].resize(n);
		for (int j = 0; j < n; j++) {
			data[i][j] = 0;
		}
	}
	//cout << n;
}

void fractal::seting(float _a, float _b, float _c, float _d, int  _deltaHeight, int _D, int seed){
	srand(seed);

	D = _D;

	//����4����
	a = _a;
	b = _b;
	c = _c;
	d = _d;
	deltaHeight = _deltaHeight;
	

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			data[i][j] = 0;
		}
	}
	data[0][0] = a;
	data[n - 1][0] = b;
	data[n - 1][n - 1] = c;
	data[0][n - 1] = d;
}

fractal::~fractal()
{
	//for (int i = 0; i < n; i++) 
	//	delete data[i];
	//delete data;

}

// ����[-num,num]֮��������  
static float randnum(float num)
{
	float max = num;
	float min = -num;
	int r;
	float x;

	r = rand();
	x = (float)(r & 0x7fff) /
		(float)0x7fff;
	return (x * (max - min) + min)>0 ? (x * (max - min) + min) : -0.5 * (x * (max - min) + min);
}


// ���㶥��߶�  
void fractal::calculate()
{
	int size = n - 1;
	int number = 1;
	int ratio = pow(2, D);
	roughness = deltaHeight;

	//timesΪ��������  
	for (int t = 0; t < times; t++) {
		// diamand�׶�  
		int offest =  size >> 1;
		for (int i = 0; i < number; i++)
			for (int j = 0; j < number; j++)
			{
				float r = randnum(.5) * roughness;
				//cout << randnum(.5) <<  " " << roughness << endl;

				int center_x = offest + (offest<<1) * i;
				int center_y = offest + (offest << 1) * j;
				data[center_x][center_y] =
					(
						data[center_x - offest][center_y - offest] +
						data[center_x - offest][center_y + offest] +
						data[center_x + offest][center_y - offest] +
						data[center_x + offest][center_y + offest]
					) / 4 + r;
				
				//cout << data[center_x][center_y] << " " << r << endl;
			}

		// square�׶�  
		for (int i = 0; i < number; i++)
			for (int j = 0; j < number; j++)
			{
				float r = randnum(.5) * roughness;
				//cout << randnum(.5) << " " << roughness << endl;
				int center_x = offest + (offest << 1) * i;
				int center_y = offest + (offest << 1) * j;
				if (!data[center_x - offest][center_y])
				{
					int x = center_x - offest;
					int y = center_y;
					if (x != 0)
						data[x][y] = (data[x - offest][y] + data[x + offest][y] + data[x][y - offest] + data[x][y + offest]) / 4 + r;
					else
						data[x][y] = (data[x + offest][y] + data[x][y - offest] + data[x][y + offest]) / 3 + r;
				}
				if (!data[center_x + offest][center_y])
				{
					int x = center_x + offest;
					int y = center_y;
					if (x != n-1)
						data[x][y] = (data[x - offest][y] + data[x + offest][y] + data[x][y - offest] + data[x][y + offest]) / 4 + r;
					else
						data[x][y] = (data[x - offest][y] + data[x][y - offest] + data[x][y + offest]) / 3 + r;
				}
				if (!data[center_x][center_y - offest])
				{
					int x = center_x;
					int y = center_y - offest;
					if (y != 0)
						data[x][y] = (data[x - offest][y] + data[x + offest][y] + data[x][y - offest] + data[x][y + offest]) / 4 + r;
					else
						data[x][y] = (data[x - offest][y] + data[x + offest][y] + data[x][y + offest]) / 3 + r;
				}
				if (!data[center_x][center_y + offest])
				{
					int x = center_x;
					int y = center_y + offest;
					if (y != n-1)
						data[x][y] = (data[x - offest][y] + data[x + offest][y] + data[x][y - offest] + data[x][y + offest]) / 4 + r;
					else
						data[x][y] = (data[x - offest][y] + data[x + offest][y] + data[x][y - offest]) / 3 + r;
				}
			}
			
		roughness = roughness / ratio;
		size >>= 1;
		number <<= 1;

	}

	//for (int i = 0; i < n; i++)
	//{
	//	for (int j = 0; j < n; j++)
	//	{
	//		cout << data[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl;
}
