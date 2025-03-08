#include "Render.h"
#include"geometry.h"
Render::Render(int width, int height) {
    shader = new Shader();

    // ��ʼ��ͶӰ��������ͶӰʾ����
    projectionMatrix = Matrix::identity();

    // Ĭ����ͼ���󣨵�λ����
    viewMatrix = Matrix::identity();
}

Render::~Render() {
    delete shader;
}

void Render::beginFrame() {
    vertexData.clear(); // ÿ֡�������
}

void Render::endFrame() {
    // �˴�������������Ż��߼�
}

void Render::setViewport(int x, int y, int w, int h) {
    viewportX = x;
    viewportY = y;
    viewportW = w;
    viewportH = h;

    // �����ӿڱ任����
    viewportMatrix = Matrix::identity();
   
}

void Render::drawTriangle(const Vec3f& pos, const Vec3f& color) {
    // ����ģ�;���λ�Ʊ任��
    
    Matrix modelMatrix = Matrix::identity();
    // MVP + Viewport �������
    Matrix mvp = projectionMatrix * viewMatrix * modelMatrix;
    Matrix finalMatrix = viewportMatrix * mvp;

    // ���������ζ��㣨ģ�Ϳռ䣩
    Vertex v1, v2, v3;
    v1.position = Vec3f(-0.2f, 0.3f, 0.0f);
    v2.position = Vec3f(0.2f, 0.3f, 0.0f);
    v3.position = Vec3f(0.0f, -0.3f, 0.0f);

    // Ӧ�ö�����ɫ��
    v1.position = shader->vertexShader(finalMatrix, v1.position);
    v2.position = shader->vertexShader(finalMatrix, v2.position);
    v3.position = shader->vertexShader(finalMatrix, v3.position);

    // ������ɫ���򵥴��ݣ�
    v1.color = v2.color = v3.color = color;

    // ���뻺����
    vertexData.insert(vertexData.end(), { v1, v2, v3 });

    // �˴�ʵ��Ӧ���ù�դ���߼�
    // rasterizer->processTriangle(v1, v2, v3);
}