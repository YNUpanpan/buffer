#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
#include"tgaimage.h"
//ģ�Ͷ�ȡ
class Model {
private:

	std::vector<Vec3f> verts_;//�洢����
	std::vector<std::vector<Vec3i> > faces_;//��Ƭ�ϼ�
	std::vector<Vec3f> norms_;//��������
	std::vector<Vec2f> uv_;//��������
	TGAImage textureMap_;//��������ͼ��Ҳ��������ͼ��������ͼӳ�䵽
	void load_texture(std::string filename, const char* suffix, TGAImage& img);//������������
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	std::vector<int> face(int idx);//��Ƭ��һ����Ƭ��{���������ţ�������������ţ�����������}
	Vec2i uv(int iface, int nvert);//�����õ���������ģ���д��uv_��
	TGAColor diffuse(Vec2i uv);//ͨ��uv���꣬ȡdiffusemap_���ҵ���Ӧ��ɫcolor
	
};

#endif //__MODEL_H__