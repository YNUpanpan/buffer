#include "tgaimage.h"//�滭���ɵ�tgaͼ���Լ�color
#include<math.h>//abs��swap����
#include<geometry.h>//���ο⣬Vec3f��Щ
#include<model.h>//��ȡģ��obj�Ŀ�
#include<random>//�������

const TGAColor black = TGAColor(0, 0, 0, 255);
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);


//�����߶�
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
	*/
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
}

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
void drawTriangle(Vec3f *screen_vert,Vec2i *screen_uv,Model* model, TGAImage& image, float intensity, float *buffer) {

	//�����ΰ�Χ�б߽�
	float xmin, xmax, ymin, ymax;
	xmin = std::min({ screen_vert[0].x, screen_vert[1].x, screen_vert[2].x });
	xmax = std::max({ screen_vert[0].x, screen_vert[1].x, screen_vert[2].x });
	ymin = std::min({ screen_vert[0].y, screen_vert[1].y, screen_vert[2].y });
	ymax = std::max({ screen_vert[0].y, screen_vert[1].y, screen_vert[2].y });

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
			float z = bary * Vec3f{ screen_vert[0].z,screen_vert[1].z,screen_vert[2].z };
			
			
			if (bary.x < 0 || bary.y < 0 || bary.z < 0) {
				continue;
			}
			else {
				if (z > buffer[y * image.get_width() + x]) {
					//ͨ���������깫ʽ������uv��ֵ���õ�������ɫ
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

//ͨ�������������Ƴ�ģ��
void drawModel(Model* model, TGAImage& image,Vec3f light_dir,float* buffer) {

	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> cur_face = model->face(i);
		Vec3f orignal_vert[3];//ԭʼ�ĵ�����[-1,1]
		Vec3f screen_vert[3];//ת�������Ļscreen������[0,width],[0,height]
		Vec2i screen_uv[3];
		for (int j = 0; j < 3; j++)
		{
			orignal_vert[j] = (model->vert(cur_face[j]));
			//��.obj�����ݷ�Χ[-1,1]ת������ͼ��ķ�Χ
			screen_vert[j].x = ((orignal_vert[j].x + 1) / 2) * image.get_width();
			screen_vert[j].y = ((orignal_vert[j].y + 1) / 2) * image.get_height();
			screen_vert[j].z = orignal_vert[j].z;
			screen_uv[j] = model->uv(i, j);
		}
		
	//�Ҳ²⣬.obj�ٴ洢�����ζ���ʱ�ǰ���һ������ģ�ʹ�ü��㷨��ʱ���ǳ���ͬһ�࣬����Ҳ�ȴ���
		Vec3f normal = ( orignal_vert[2] - orignal_vert[0] ) ^( orignal_vert[1] - orignal_vert[0] );
		normal.normalize();//��һ���͹��������õ�����ǿ��

		float intensity = normal * light_dir;//����ǿ��

		if (intensity > 0) {
			//���Ƴ����������
			
			drawTriangle(screen_vert, screen_uv,model, image, intensity, buffer);
		}
	}
}
int main(int argc, char** argv) {

	int imageHeight = 1000;
	int imageWidth = 1000;
	TGAImage image(imageWidth, imageHeight, TGAImage::RGB);
	Vec3f light_dir = Vec3f(0, 0, -1);//���շ���

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