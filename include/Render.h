#pragma once
#include "Shader.h"
#include <vector>

// �Զ�����Ⱦ���ࣨ��װͼ�λ����߼���
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
        Vec3f position; // ģ�Ϳռ�����
        Vec3f normal;    // ������ɫ
        Vec3f color;    // ������ɫ
    };

    Shader* shader;                 // ��ɫ��ʵ��
    std::vector<Vertex> vertexData; // ���㻺����

    Matrix projectionMatrix;  // ͶӰ����
    Matrix viewMatrix;        // ��ͼ����
    Matrix viewportMatrix;    // �ӿھ���

    int viewportX, viewportY, viewportW, viewportH;

    void applyTransformations();
};