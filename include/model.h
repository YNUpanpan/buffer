#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
#include"tgaimage.h"
//模型读取
class Model {
private:

	std::vector<Vec3f> verts_;//存储顶点
	std::vector<std::vector<Vec3i> > faces_;//面片合集
	std::vector<Vec3f> norms_;//法线向量
	std::vector<Vec2f> uv_;//纹理坐标
	TGAImage textureMap_;//漫反射贴图，也就是纹理图，将纹理图映射到
	void load_texture(std::string filename, const char* suffix, TGAImage& img);//加载纹理坐标
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	std::vector<int> face(int idx);//面片，一个面片由{三个顶点编号，三个法向量编号，三个纹理编号}
	Vec2i uv(int iface, int nvert);//用来得到纹理坐标的，再写进uv_里
	TGAColor diffuse(Vec2i uv);//通过uv坐标，取diffusemap_里找到对应颜色color
	
};

#endif //__MODEL_H__