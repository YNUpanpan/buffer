#include "tgaimage.h"//绘画生成的tga图像以及color
#include<math.h>//abs和swap函数
#include<geometry.h>//几何库，Vec3f这些
#include<model.h>//读取模型obj的库
#include<random>//随机函数
#include <iostream>

const TGAColor black = TGAColor(0, 0, 0, 255);
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);



Vec3f eye(1, 1, 4);//摄像机所在的位置
Vec3f center(0, 0, 0);//摄像机所朝向的中心位置
Vec3f up{ 0,1,0 };//定义一个向上的方向向量，在视图矩阵中进行转换使用
Vec3f light_dir = Vec3f(0, 0, 1);//光照方向


/*绘制线段
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
	*
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
}*/

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
void drawTriangle(Vec3f *screen_vert,Vec2f *screen_uv, Vec3f * n,Vec3f light,Model* model, TGAImage& image, float *buffer) {


	//三角形包围盒边界
	float xmin, xmax, ymin, ymax;
	
	xmin =std::max(.0f,std::min({ screen_vert[0].x, screen_vert[1].x, screen_vert[2].x }));
	xmax = std::min(float(image.get_width()), std::max({screen_vert[0].x, screen_vert[1].x, screen_vert[2].x}));
	ymin = std::max(.0f, std::min({ screen_vert[0].y, screen_vert[1].y, screen_vert[2].y }));
	ymax = std::min(float(image.get_height()), std::max({ screen_vert[0].y, screen_vert[1].y, screen_vert[2].y }));


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

			if (bary.x < 0 || bary.y < 0 || bary.z < 0) {
				continue;
			}


			float z = bary * Vec3f{ screen_vert[0].z,screen_vert[1].z,screen_vert[2].z };
			if (z < buffer[y * image.get_width() + x]) continue;
			
			buffer[x + y * image.get_width()] = z;
			
			
			
			Vec3f n_real;
			for (int i = 0; i < 3; i++)
			{
				n_real[0] += n[i][0] * bary[i];
				n_real[1] += n[i][1] * bary[i];
				n_real[2] += n[i][2] * bary[i];
			}
			n_real=n_real.normalize();

			float intensity;
			intensity = n_real * light;
			if (intensity < 0)
				intensity = 0;
			
				
					//通过重心坐标公式，计算uv的值，得到纹理颜色
					Vec2f uv;
						uv[0] = screen_uv[0].x * bary.x + screen_uv[1].x * bary.y + screen_uv[2].x * bary.z;
						uv[1] = screen_uv[0].y * bary.x + screen_uv[1].y * bary.y + screen_uv[2].y * bary.z;
					TGAColor color = model->diffuse(uv);
					TGAColor c = TGAColor(color.r * intensity, color.g * intensity, color.b * intensity, 255);
					buffer[y * image.get_width() + x] = z;
					image.set(x, y, c);
				
			
		}
	}
}

//视口变换,将NDC坐标转化为screen坐标

//在图像的xy开始绘画，宽度为w，高度为h，深度范围为[-depth/2,depth/2]
Matrix viewport(int x, int y, int w, int h,int depth) {
	Matrix m = Matrix::identity(4);
	m[0][3] = x + w / 2.f;
	m[1][3] = y + h / 2.f;
	m[2][3] = depth/2.0f;

	m[0][0] = w / 2.f;
	m[1][1] = h / 2.f;
	m[2][2] = depth/2.0f;
	return m;
}

//model+view变换，指定相机位置，朝指定方向进行渲染

Matrix ModelView(Vec3f eye, Vec3f center, Vec3f up) {


	Vec3f z = (eye - center).normalize();// 视线方向
	Vec3f x = (up ^ z).normalize();// 右方向
	Vec3f y = (z ^ x).normalize(); // 上方向
	Matrix Minv = Matrix::identity(4);
	for (int i = 0; i < 3; i++) {
		Minv[0][i] = x[i];
		Minv[1][i] = y[i];
		Minv[2][i] = z[i];
		Minv[i][3] = -center[i];//这个是困扰我最最最最久的//
	}
	return Minv;
}

//投影
Matrix Projection(float value) {
	Matrix m=Matrix::identity(4);

	m[3][2] = value;
	return m;
}

//透视除法
Matrix proAfter(Matrix m) {
	m[0][0] = m[0][0] / m[3][0];
	m[1][0] = m[1][0] / m[3][0];
	m[2][0] = m[2][0] / m[3][0];
	m[3][0] = 1;
	return m;
}

//通过三角形面板绘制出模型
void drawModel(Model* model, TGAImage& image,Vec3f light_dir,float* buffer) {

	//进行MVP以及视口变换
	Matrix ModelViewM = ModelView(eye, center, up);
	Matrix viewPortM = viewport(image.get_width()/8, image.get_height()/8, image.get_width()*3/4, image.get_height()*3/4, 2);
	Matrix ProjectionM = Projection(-1.f / (eye - center).norm());
	
	Matrix transform_Mat = ProjectionM * ModelViewM;
	//透视除法

	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> cur_face = model->face(i);
		Vec3f orignal_vert[3];//原始的点坐标[-1,1]
		Vec3f screen_vert[3];//转化后的屏幕screen点坐标[0,width],[0,height]
		Vec3f n[3];//三个顶点法线向量
		Vec2f screen_uv[3];
		float intensity[3];//顶点光线强度
		for (int j = 0; j < 3; j++)
		{
			orignal_vert[j] = (model->vert(cur_face[j]));
			
			Matrix screen_vertMat0= Matrix(orignal_vert[j]);
			Matrix screen_vertMat1 = ModelViewM * screen_vertMat0;
			Matrix screen_vertMat2 = ProjectionM  * screen_vertMat1;
			Matrix screen_vertMat3= viewPortM * screen_vertMat2;
			Matrix screen_vertMat = proAfter(screen_vertMat3);
			
			
			
			screen_vert[j] = Vec3f{ screen_vertMat[0][0],screen_vertMat[1][0] ,screen_vertMat[2][0]};
			screen_uv[j] = model->uv(i, j);
			n[j] = model->norm(i, j);
			Matrix tempMat(n[j]);
			Matrix mm = ModelViewM.inverse().transpose() * tempMat;
			n[j]= Vec3f{ mm[0][0],mm[1][0] ,mm[2][0] }.normalize();
		}
		Matrix tempMat1(light_dir);
		Matrix lM =  ModelViewM * tempMat1;
		Vec3f l = Vec3f{ lM[0][0],lM[1][0], lM[2][0] }.normalize();
			//绘制出三角形面板
			drawTriangle(screen_vert, screen_uv,n,l,model, image, buffer);	
	}
}

int main(int argc, char** argv) {

	int imageHeight = 1000;
	int imageWidth = 1000;
	TGAImage image(imageWidth, imageHeight, TGAImage::RGB);
	

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