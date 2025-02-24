#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <fstream>//文件输入输出操作


/*
2025/2/24
用来读取和绘画tga文件
*/


#pragma pack(push,1)//设置结构体的内存对齐方式，确保结构体成员按照1字节对齐，在读取文件时结构体的布局和文件中的二进制数据能够正确对应。
struct TGA_Header {
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;//每像素位数（如24=RGB，32=RGBA）
	char  imagedescriptor;
};
#pragma pack(pop)


//tga文件一个像素的颜色结构体，包括rgb三个颜色和a透明度，
struct TGAColor {
	union {
		struct {
			unsigned char b, g, r, a;
		};
		unsigned char raw[4];//字节数组形式
		unsigned int val;
	};
	int bytespp;//每像素字节数（1=灰度，3=RGB，4=RGBA）

	TGAColor() : val(0), bytespp(1) {
	}

	//构造函数
	TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : b(B), g(G), r(R), a(A), bytespp(4) {
	}

	TGAColor(int v, int bpp) : val(v), bytespp(bpp) {
	}

	TGAColor(const TGAColor& c) : val(c.val), bytespp(c.bytespp) {
	}

	TGAColor(const unsigned char* p, int bpp) : val(0), bytespp(bpp) {
		for (int i = 0; i < bpp; i++) {
			raw[i] = p[i];
		}
	}


	// 赋值运算符
	TGAColor& operator =(const TGAColor& c) {
		if (this != &c) {
			bytespp = c.bytespp;
			val = c.val;
		}
		return *this;
	}
};


class TGAImage {
protected:
	unsigned char* data;
	int width;
	int height;
	int bytespp;

	bool   load_rle_data(std::ifstream& in);
	bool unload_rle_data(std::ofstream& out);
public:
	enum Format {
		GRAYSCALE = 1, RGB = 3, RGBA = 4
	};

	TGAImage();
	TGAImage(int w, int h, int bpp);
	TGAImage(const TGAImage& img);
	bool read_tga_file(const char* filename);
	bool write_tga_file(const char* filename, bool rle = true);
	bool flip_horizontally();
	bool flip_vertically();
	bool scale(int w, int h);
	TGAColor get(int x, int y);
	bool set(int x, int y, TGAColor c);
	~TGAImage();
	TGAImage& operator =(const TGAImage& img);
	int get_width();
	int get_height();
	int get_bytespp();
	unsigned char* buffer();
	void clear();
};

#endif //__IMAGE_H__