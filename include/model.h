#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

//模型读取
class Model {
private:

	std::vector<Vec3f> verts_;//存储顶点
	std::vector<std::vector<int> > faces_;//面片合集
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	std::vector<int> face(int idx);//面片，一个面片由三个顶点组成
};

#endif //__MODEL_H__