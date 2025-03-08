#pragma once
#include "Shader.h"
#include <vector>

// 自定义渲染器类（封装图形绘制逻辑）
class Render {
public:
    Render(int width, int height);
    ~Render();

    void beginFrame();
    void endFrame();

    void setViewport(int x, int y, int w, int h);
    void drawTriangle(const Vec3f& position, const Vec3f& color);

private:
    struct Vertex {
        Vec3f position; // 模型空间坐标
        Vec3f normal;    // 顶点颜色
        Vec3f color;    // 顶点颜色
    };

    Shader* shader;                 // 着色器实例
    std::vector<Vertex> vertexData; // 顶点缓冲区

    Matrix projectionMatrix;  // 投影矩阵
    Matrix viewMatrix;        // 视图矩阵
    Matrix viewportMatrix;    // 视口矩阵

    int viewportX, viewportY, viewportW, viewportH;

    void applyTransformations();
};