#include "tgaimage.h"//�滭���ɵ�tgaͼ���Լ�color
#include<math.h>//abs��swap����
#include<geometry.h>//���ο⣬Vec3f��Щ
#include<model.h>//��ȡģ��obj�Ŀ�
#include<random>//�������
#include <iostream>

const TGAColor black = TGAColor(0, 0, 0, 255);
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);



Vec3f eye(1, 1, 4);//��������ڵ�λ��
Vec3f center(0, 0, 0);//����������������λ��
Vec3f up{ 0,1,0 };//����һ�����ϵķ�������������ͼ�����н���ת��ʹ��
Vec3f light_dir = Vec3f(0, 0, 1);//���շ���


/*�����߶�
void drawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	//���б�ʴ���1�����������߶β������̫�٣�ϡ�裬����Ҫת��
	bool steep = false;
	if (abs(y1 - y0) > abs(x1 - x0)) {
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	//��Ϊ����滭��forѭ����Ҫ++������Ҫ�������һ滭�����x0<x1,Ҫ�Ƚ��н���
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	/*
	* ����Ĵ���Ҳ�ԣ��������������߶Σ����Ǹ���float������࣬ȫ����Ϊint����
	float k = abs((y1 - y0) / float(x1 - x0));//float k = abs(y1 - y0) / (x1 - x0);ֱ������д�ᱻĬ��ȡ��
	float cur_y = y0;
	float error=0;//��ԭ����ȡ��������ȡ������������������ȡ�������>0.5����ȡ����<0.5����ȡ��
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
	//Bresenham�㷨��ֻ�����������ܸ���
	int dx = x1 - x0;
	int dy = y1 - y0;
	int error = abs(dy) * 2;//��2����ΪҪ��������ȡ��
	int Sumerror = 0;//�ۼƲ�ֵ
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

//ͨ���߶λ��Ƴ�ģ�͵�����
void drawModelLine(Model *model,TGAImage& image) {//һ����ָ��*,һ����&����
	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> cur_face = model->face(i);//��ǰ��Ƭ
		for (int j = 0; j < 3; j++)
		{
			//ȡ��ǰ��Ƭ��������������л����߶�
			Vec3f v0 = model->vert(cur_face[j]);
			Vec3f v1 = model->vert(cur_face[(j + 1) % 3]);

			//�õ�������ĺ������꣬����Ϊʲô+1/2����������Ҫ��.obj�ļ��е����ݷ�Χ[-1,1]->��ǰimage�ķ�Χ
			int x0 = (1 + v0.x) * image.get_width() / 2;
			int y0 = (1 + v0.y) * image.get_height() / 2;
			int x1 = (1 + v1.x) * image.get_width() / 2;
			int y1 = (1 + v1.y) * image.get_height() / 2;

			drawLine(x0, y0, x1, y1, image, white);
		}
	}
}*/

//��������ε���������,���ݹ�ʽuAB+vAC+PA=0������������
Vec3f barycentric(Vec2f AB, Vec2f AC, Vec2f PA) {
	Vec3f bary;
	bary = Vec3f{ AB.x,AC.x,PA.x } ^ Vec3f{ AB.y,AC.y,PA.y };//����ˣ��õ�uv1
	if (abs(bary.z) < 1e-6) {//bary.z�����������������������bary.z<1e-6ʱ��˵������������Ѿ��˻���������һ��ֱ��
		return Vec3f{ -1,1,1 };
	}
	//������������
	 return Vec3f(1.f - (bary.x + bary.y) / bary.z, bary.x / bary.z, bary.y / bary.z);
}

//�������������
void drawTriangle(Vec3f *screen_vert,Vec2f *screen_uv, Vec3f * n,Vec3f light,Model* model, TGAImage& image, float *buffer) {


	//�����ΰ�Χ�б߽�
	float xmin, xmax, ymin, ymax;
	
	xmin =std::max(.0f,std::min({ screen_vert[0].x, screen_vert[1].x, screen_vert[2].x }));
	xmax = std::min(float(image.get_width()), std::max({screen_vert[0].x, screen_vert[1].x, screen_vert[2].x}));
	ymin = std::max(.0f, std::min({ screen_vert[0].y, screen_vert[1].y, screen_vert[2].y }));
	ymax = std::min(float(image.get_height()), std::max({ screen_vert[0].y, screen_vert[1].y, screen_vert[2].y }));


	//AB,AC�����������ں�������������
	Vec2f AB = Vec2f{ screen_vert[1].x - screen_vert[0].x ,screen_vert[1].y - screen_vert[0].y };
	Vec2f AC = Vec2f{ screen_vert[2].x - screen_vert[0].x ,screen_vert[2].y - screen_vert[0].y };

	//������Χ�У������������ڲ��ĵ���л���
	for (int x = xmin; x < xmax; x++)
	{
		for (int y = ymin; y < ymax; y++)
		{
			
			//�����������깫ʽ�õ���������
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
			
				
					//ͨ���������깫ʽ������uv��ֵ���õ�������ɫ
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

//�ӿڱ任,��NDC����ת��Ϊscreen����

//��ͼ���xy��ʼ�滭�����Ϊw���߶�Ϊh����ȷ�ΧΪ[-depth/2,depth/2]
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

//model+view�任��ָ�����λ�ã���ָ�����������Ⱦ

Matrix ModelView(Vec3f eye, Vec3f center, Vec3f up) {


	Vec3f z = (eye - center).normalize();// ���߷���
	Vec3f x = (up ^ z).normalize();// �ҷ���
	Vec3f y = (z ^ x).normalize(); // �Ϸ���
	Matrix Minv = Matrix::identity(4);
	for (int i = 0; i < 3; i++) {
		Minv[0][i] = x[i];
		Minv[1][i] = y[i];
		Minv[2][i] = z[i];
		Minv[i][3] = -center[i];//�������������������õ�//
	}
	return Minv;
}

//ͶӰ
Matrix Projection(float value) {
	Matrix m=Matrix::identity(4);

	m[3][2] = value;
	return m;
}

//͸�ӳ���
Matrix proAfter(Matrix m) {
	m[0][0] = m[0][0] / m[3][0];
	m[1][0] = m[1][0] / m[3][0];
	m[2][0] = m[2][0] / m[3][0];
	m[3][0] = 1;
	return m;
}

//ͨ�������������Ƴ�ģ��
void drawModel(Model* model, TGAImage& image,Vec3f light_dir,float* buffer) {

	//����MVP�Լ��ӿڱ任
	Matrix ModelViewM = ModelView(eye, center, up);
	Matrix viewPortM = viewport(image.get_width()/8, image.get_height()/8, image.get_width()*3/4, image.get_height()*3/4, 2);
	Matrix ProjectionM = Projection(-1.f / (eye - center).norm());
	
	Matrix transform_Mat = ProjectionM * ModelViewM;
	//͸�ӳ���

	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> cur_face = model->face(i);
		Vec3f orignal_vert[3];//ԭʼ�ĵ�����[-1,1]
		Vec3f screen_vert[3];//ת�������Ļscreen������[0,width],[0,height]
		Vec3f n[3];//�������㷨������
		Vec2f screen_uv[3];
		float intensity[3];//�������ǿ��
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
			//���Ƴ����������
			drawTriangle(screen_vert, screen_uv,n,l,model, image, buffer);	
	}
}

int main(int argc, char** argv) {

	int imageHeight = 1000;
	int imageWidth = 1000;
	TGAImage image(imageWidth, imageHeight, TGAImage::RGB);
	

	//�������buffer
	float* zbuffer = new float[imageWidth * imageHeight];
	for (int i = 0; i < imageWidth * imageHeight; i++)
	{
		zbuffer[i] = -std::numeric_limits<float>::max();//��ʼ��Ϊ��Сֵ
	}
	//��ȡģ��
	Model* model = new Model("model/african_head.obj");

	//ͨ���߶λ��Ƴ�ģ�͵�����
	//drawModelLine(model,image);
	
	//ͨ�������������Ƴ�ģ��
	drawModel(model, image,light_dir,zbuffer);

	/*drawLine(13, 20, 80, 40, image, red);
	drawLine(20, 13, 40, 80, image, green);
	drawLine(80, 40, 13, 20, image, blue);*/
	image.flip_vertically(); // ���·�ת����Ϊ��image��ԭ�������Ͻǣ������ǿ�ͼʱԭ�������½�
	image.write_tga_file("output/output.tga");
	return 0;
}