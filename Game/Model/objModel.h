#pragma once

#include "model.h"

class ObjModel : public Model
{
public:
	bool loadModel(const std::string& file);
	void draw() const;
	void drawDepthMap() const;

private:
	vector<SubObj*> objs;
};