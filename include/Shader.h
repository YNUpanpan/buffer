#pragma once
#include "geometry.h"

// ��ɫ���ࣨ�������任��
class Shader {
public:
    // ������ɫ����Ӧ�ñ任����
    static Vec3f vertexShader(const Matrix& mvp, const Vec3f& vertex) {
        
        Vec4f pos= mvp * embed<4>(vertex);
        // ͸�ӳ���������ͶӰ��ʡ�ԣ�
        pos = pos/pos[3];
        return Vec3f(pos[0], pos[1], pos[2]);
    }
};