#include "objModel.h"
#include  "../resource.h"



void ObjModel::draw() const
{
	for (SubObj* s : objs) {
		s->draw();
	}
}

void ObjModel::drawDepthMap() const
{
	for (SubObj* s : objs) {
		s->draw();
	}
}

bool ObjModel::loadModel(const std::string& file)
{
	ResourceManager::loadOBJ(file, objs);
	pos_info = glm::vec3((objs[0]->xmax + objs[0]->xmin) / 2, (objs[0]->ymax + objs[0]->ymin) / 2, (objs[0]->ymax + objs[0]->ymin) / 2);
	len_info = glm::vec3((objs[0]->xmax - objs[0]->xmin) / 2, (objs[0]->ymax - objs[0]->ymin) / 2, (objs[0]->ymax - objs[0]->ymin) / 2);

	return true;
}

