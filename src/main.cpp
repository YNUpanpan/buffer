#include "tgaimage.h"//绘画生成的tga图像以及color
#include<math.h>//abs和swap函数
#include<geometry.h>//几何库，Vec3f这些
#include<model.h>//读取模型obj的库
#include<random>//随机函数

const TGAColor black = TGAColor(0, 0, 0, 255);
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);


//绘制线段
void drawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	//如果斜率大于1，画出来的线段采样点会太少，稀疏，所以要转换
	bool steep = false;
	if (abs(y1 - y0) > abs(x1 - x0)) {
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}

	//因为后面绘画的for循环需要++，所以要从左往右绘画，如果x0<x1,要先进行交换
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	/*
	* 下面的代码也对，可以正常绘制线段，但是浮点float运算过多，全部改为int运算
	float k = abs((y1 - y0) / float(x1 - x0));//float k = abs(y1 - y0) / (x1 - x0);直接这样写会被默认取整
	float cur_y = y0;
	float error=0;//误差，原来的取整是整数取整，现在用四舍五入取整，误差>0.5向上取整，<0.5向下取整
	for (int cur_x = x0; cur_x < x1; cur_x++)
	{
		if(steep){
			image.set(int(cur_y), cur_x, color); }
		else { image.set(cur_x, int(cur_y), color); }

		error += k;
		if (error > 0.5) {
			cur_y += (y1 > y0 ? 1 : -1);
			error--;
		}
		cur_y += k;
	}
	*/
	//Bresenham算法，只有整数，性能更好
	int dx = x1 - x0;
	int dy = y1 - y0;
	int error = abs(dy) * 2;//×2是因为要四舍五入取整
	int Sumerror = 0;//累计差值
	int  cur_y = y0;
	for (int cur_x = x0; cur_x < x1; cur_x++)
	{
		if (steep) {
			image.set(cur_y, cur_x, color);
		}
		else { 
			image.set(cur_x, cur_y, color); 
		}
		Sumerror += error;
		if (Sumerror > dx) {
			cur_y += (y1 > y0 ? 1 : -1);
			Sumerror -= (dx * 2);
		}
	}
}

//通过线段绘制出模型的轮廓
void drawModelLine(Model *model,TGAImage& image) {//一个是指针*,一个是&引用
	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> cur_face = model->face(i);//当前面片
		for (int j = 0; j < 3; j++)
		{
			//取当前面片的其中两个点进行绘制线段
			Vec3f v0 = model->vert(cur_face[j]);
			Vec3f v1 = model->vert(cur_face[(j + 1) % 3]);

			//得到两个点的横纵坐标，至于为什么+1/2，是我们需要把.obj文件中的数据范围[-1,1]->当前image的范围
			int x0 = (1 + v0.x) * image.get_width() / 2;
			int y0 = (1 + v0.y) * image.get_height() / 2;
			int x1 = (1 + v1.x) * image.get_width() / 2;
			int y1 = (1 + v1.y) * image.get_height() / 2;
			drawLine(x0, y0, x1, y1, image, white);
		}
	}
}

//求出三角形的重心坐标,根据公式uAB+vAC+PA=0来求重心坐标
Vec3f barycentric(Vec2f AB, Vec2f AC, Vec2f PA) {
	Vec3f bary;
	bary = Vec3f{ AB.x,AC.x,PA.x } ^ Vec3f{ AB.y,AC.y,PA.y };//做叉乘，得到uv1
	if (abs(bary.z) < 1e-6) {//bary.z代表三角形面积的两倍，当bary.z<1e-6时，说明这个三角形已经退化，几乎成一条直线
		return Vec3f{ -1,1,1 };
	}
	//返回重心坐标
	 return Vec3f(1.f - (bary.x + bary.y) / bary.z, bary.x / bary.z, bary.y / bary.z);
}

//绘制三角形面板
void drawTriangle(Vec3f *screen_vert,Vec2i *screen_uv,Model* model, TGAImage& image, float intensity, float *buffer) {

	//三角形包围盒边界
	float xmin, xmax, ymin, ymax;
	xmin = std::min({ screen_vert[0].x, screen_vert[1].x, screen_vert[2].x });
	xmax = std::max({ screen_vert[0].x, screen_vert[1].x, screen_vert[2].x });
	ymin = std::min({ screen_vert[0].y, screen_vert[1].y, screen_vert[2].y });
	ymax = std::max({ screen_vert[0].y, screen_vert[1].y, screen_vert[2].y });

	//AB,AC两条向量用于后面求重心坐标
	Vec2f AB = Vec2f{ screen_vert[1].x - screen_vert[0].x ,screen_vert[1].y - screen_vert[0].y };
	Vec2f AC = Vec2f{ screen_vert[2].x - screen_vert[0].x ,screen_vert[2].y - screen_vert[0].y };

	//遍历包围盒，将在三角形内部的点进行绘制
	for (int x = xmin; x < xmax; x++)
	{
		for (int y = ymin; y < ymax; y++)
		{
			//根据重心坐标公式得到重心坐标
			Vec2f PA = Vec2f{ screen_vert[0].x - x, screen_vert[0].y - y };
			Vec3f bary = barycentric(AB, AC, PA);
			float z = bary * Vec3f{ screen_vert[0].z,screen_vert[1].z,screen_vert[2].z };
			
			
			if (bary.x < 0 || bary.y < 0 || bary.z < 0) {
				continue;
			}
			else {
				if (z > buffer[y * image.get_width() + x]) {
					//通过重心坐标公式，计算uv的值，得到纹理颜色
					Vec2i uv;
						uv[0] = screen_uv[0].x * bary.x + screen_uv[1].x * bary.y + screen_uv[2].x * bary.z;
						uv[1] = screen_uv[0].y * bary.x + screen_uv[1].y * bary.y + screen_uv[2].y * bary.z;
					TGAColor color = model->diffuse(uv);
					buffer[y * image.get_width() + x] = z;
					image.set(x, y, color);
				}
			}
		}
	}
}

//通过三角形面板绘制出模型
void drawModel(Model* model, TGAImage& image,Vec3f light_dir,float* buffer) {

	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> cur_face = model->face(i);
		Vec3f orignal_vert[3];//原始的点坐标[-1,1]
		Vec3f screen_vert[3];//转化后的屏幕screen点坐标[0,width],[0,height]
		Vec2i screen_uv[3];
		for (int j = 0; j < 3; j++)
		{
			orignal_vert[j] = (model->vert(cur_face[j]));
			//将.obj的数据范围[-1,1]转化到该图像的范围
			screen_vert[j].x = ((orignal_vert[j].x + 1) / 2) * image.get_width();
			screen_vert[j].y = ((orignal_vert[j].y + 1) / 2) * image.get_height();
			screen_vert[j].z = orignal_vert[j].z;
			screen_uv[j] = model->uv(i, j);
		}
		
	//我猜测，.obj再存储三角形顶点时是按照一定规则的，使得计算法线时都是朝向同一侧，不过也先存疑
		Vec3f normal = ( orignal_vert[2] - orignal_vert[0] ) ^( orignal_vert[1] - orignal_vert[0] );
		normal.normalize();//归一化和光线求点积得到光线强度

		float intensity = normal * light_dir;//光线强度

		if (intensity > 0) {
			//绘制出三角形面板
			
			drawTriangle(screen_vert, screen_uv,model, image, intensity, buffer);
		}
	}
}
int main(int argc, char** argv) {

	int imageHeight = 1000;
	int imageWidth = 1000;
	TGAImage image(imageWidth, imageHeight, TGAImage::RGB);
	Vec3f light_dir = Vec3f(0, 0, -1);//光照方向

	//建立深度buffer
	float* zbuffer = new float[imageWidth * imageHeight];
	for (int i = 0; i < imageWidth * imageHeight; i++)
	{
		zbuffer[i] = -std::numeric_limits<float>::max();//初始化为最小值
	}
	//读取模型
	Model* model = new Model("model/african_head.obj");

	//通过线段绘制出模型的轮廓
	//drawModelLine(model,image);
	
	//通过三角形面板绘制出模型
	drawModel(model, image,light_dir,zbuffer);

	/*drawLine(13, 20, 80, 40, image, red);
	drawLine(20, 13, 40, 80, image, green);
	drawLine(80, 40, 13, 20, image, blue);*/
	image.flip_vertically(); // 上下翻转，因为在image中原点在左上角，二我们看图时原点在左下角
	image.write_tga_file("output/output.tga");
	return 0;
}