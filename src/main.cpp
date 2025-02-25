#include "tgaimage.h"//�滭���ɵ�tgaͼ���Լ�color
#include<math.h>//abs��swap����
#include<geometry.h>//���ο⣬Vec3f��Щ
#include<model.h>//��ȡģ��obj�Ŀ�


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
	* ����Ĵ���Ҳ�ԣ��������������߶Σ����Ǹ���float������࣬����ȫ����Ϊint����
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
		else { image.set(cur_x, cur_y, color); }

		Sumerror += error;
		if (Sumerror > dx) {
			cur_y += (y1 > y0 ? 1 : -1);
			Sumerror -= (dx * 2);
		}
	}

}

int main(int argc, char** argv) {

	int imageHeight = 1000;
	int imageWidth = 1000;

	TGAImage image(imageWidth, imageHeight, TGAImage::RGB);

	//��ȡģ��
	Model* model = new Model("model/african_head.obj");

	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> cur_face = model->face(i);//��ǰ��Ƭ

		for (int j = 0; j < 3; j++)
		{
			//ȡ��ǰ��Ƭ��������������л����߶�
			Vec3f v0 = model->vert(cur_face[j]);
			Vec3f v1 = model->vert(cur_face[(j + 1) % 3]);

			//�õ�������ĺ������꣬����Ϊʲô+1/2����������Ҫ��.obj�ļ��е����ݷ�Χ[-1,1]->��ǰimage�ķ�Χ
			int x0 = (1 + v0.x) * imageWidth / 2;
			int y0 = (1 + v0.y) * imageHeight / 2;
			int x1 = (1 + v1.x) * imageWidth / 2;
			int y1 = (1 + v1.y) * imageHeight / 2;


			drawLine(x0, y0, x1, y1, image, white);
		}
	}

	/*drawLine(13, 20, 80, 40, image, red);
	drawLine(20, 13, 40, 80, image, green);
	drawLine(80, 40, 13, 20, image, blue);*/
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output/output.tga");
	return 0;
}