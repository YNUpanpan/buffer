#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

//ģ�Ͷ�ȡ
class Model {
private:

	std::vector<Vec3f> verts_;//�洢����
	std::vector<std::vector<int> > faces_;//��Ƭ�ϼ�
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	std::vector<int> face(int idx);//��Ƭ��һ����Ƭ�������������
};

#endif //__MODEL_H__