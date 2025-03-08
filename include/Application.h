#pragma once
#include "SDL.h"
#include "Render.h"
#include "Button.h"

// Ӧ�ó������ࣨ�����ںͿؼ���
class Application {
public:
    Application(int width, int height);
    ~Application();

    void run(); // ��ѭ��

private:
    void processEvents(); // �¼�����
    void update();        // ״̬����
    void render();        // ��Ⱦ����

    SDL_Window* window;    // SDL����
    SDL_Renderer* sdlRenderer; // SDL��Ⱦ��

    Render* renderer;      // �Զ�����Ⱦ��
    Button* btnTriangle;   // ���������ΰ�ť
    Button* btnTwoTriangles; // ����˫�����ΰ�ť

    int screenWidth;
    int screenHeight;

    bool showBlueTriangle = false;
    bool showGreenTriangles = false;
};