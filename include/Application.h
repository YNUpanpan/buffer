#pragma once
#include "SDL.h"
#include "Render.h"
#include "Button.h"

// 应用程序主类（管理窗口和控件）
class Application {
public:
    Application(int width, int height);
    ~Application();

    void run(); // 主循环

private:
    void processEvents(); // 事件处理
    void update();        // 状态更新
    void render();        // 渲染调度

    SDL_Window* window;    // SDL窗口
    SDL_Renderer* sdlRenderer; // SDL渲染器

    Render* renderer;      // 自定义渲染器
    Button* btnTriangle;   // 绘制三角形按钮
    Button* btnTwoTriangles; // 绘制双三角形按钮

    int screenWidth;
    int screenHeight;

    bool showBlueTriangle = false;
    bool showGreenTriangles = false;
};