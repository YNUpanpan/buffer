#include "Render.h"
#include"geometry.h"
Render::Render(int width, int height) {
    shader = new Shader();

    // 初始化投影矩阵（正交投影示例）
    projectionMatrix = Matrix::identity();

    // 默认视图矩阵（单位矩阵）
    viewMatrix = Matrix::identity();
}

Render::~Render() {
    delete shader;
}

void Render::beginFrame() {
    vertexData.clear(); // 每帧清空数据
}

void Render::endFrame() {
    // 此处可添加批处理优化逻辑
}

void Render::setViewport(int x, int y, int w, int h) {
    viewportX = x;
    viewportY = y;
    viewportW = w;
    viewportH = h;

    // 计算视口变换矩阵
    viewportMatrix = Matrix::identity();
   
}

void Render::drawTriangle(const Vec3f& pos, const Vec3f& color) {
    // 定义模型矩阵（位移变换）
    
    Matrix modelMatrix = Matrix::identity();
    // MVP + Viewport 矩阵计算
    Matrix mvp = projectionMatrix * viewMatrix * modelMatrix;
    Matrix finalMatrix = viewportMatrix * mvp;

    // 生成三角形顶点（模型空间）
    Vertex v1, v2, v3;
    v1.position = Vec3f(-0.2f, 0.3f, 0.0f);
    v2.position = Vec3f(0.2f, 0.3f, 0.0f);
    v3.position = Vec3f(0.0f, -0.3f, 0.0f);

    // 应用顶点着色器
    v1.position = shader->vertexShader(finalMatrix, v1.position);
    v2.position = shader->vertexShader(finalMatrix, v2.position);
    v3.position = shader->vertexShader(finalMatrix, v3.position);

    // 设置颜色（简单传递）
    v1.color = v2.color = v3.color = color;

    // 加入缓冲区
    vertexData.insert(vertexData.end(), { v1, v2, v3 });

    // 此处实际应调用光栅化逻辑
    // rasterizer->processTriangle(v1, v2, v3);
}