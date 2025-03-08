#pragma once
#include "geometry.h"

// 着色器类（处理矩阵变换）
class Shader {
public:
    // 顶点着色器：应用变换矩阵
    static Vec3f vertexShader(const Matrix& mvp, const Vec3f& vertex) {
        
        Vec4f pos= mvp * embed<4>(vertex);
        // 透视除法（正交投影可省略）
        pos = pos/pos[3];
        return Vec3f(pos[0], pos[1], pos[2]);
    }
};