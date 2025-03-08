#include "Application.h"
#include"geometry.h"
Application::Application(int width, int height)
    : screenWidth(width), screenHeight(height) {

    // 初始化SDL
    SDL_Init(SDL_INIT_VIDEO);

    // 创建窗口
    window = SDL_CreateWindow("MVP Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN);

    // 创建SDL渲染器（仅供UI使用）
    sdlRenderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED);

    // 初始化自定义渲染器（使用独立渲染目标）
    renderer = new Render(width, height);

    // 创建按钮（左侧20%区域）
    int btnWidth = width * 0.2;
    btnTriangle = new Button(0, 0, btnWidth, 50, "Draw Blue");
    btnTwoTriangles = new Button(0, 60, btnWidth, 50, "Draw Green");
}

Application::~Application() {
    delete renderer;
    delete btnTriangle;
    delete btnTwoTriangles;
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Application::processEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        }

        // 处理按钮点击（将事件坐标传递给按钮）
        btnTriangle->handleEvent(e);
        btnTwoTriangles->handleEvent(e);
    }
}

void Application::update() {
    // 检测按钮状态
    if (btnTriangle->isClicked()) {
        showBlueTriangle = !showBlueTriangle;
        btnTriangle->resetState();
        std::cout << "first"<<std::endl;
    }
    if (btnTwoTriangles->isClicked()) {
        showGreenTriangles = !showGreenTriangles;
        btnTwoTriangles->resetState();
        std::cout << "second" << std::endl;
    }
}

void Application::render() {
    // 清空缓冲区
    SDL_SetRenderDrawColor(sdlRenderer, 45, 45, 45, 255);
    SDL_RenderClear(sdlRenderer);

    // 绘制按钮（使用SDL原生渲染）
    btnTriangle->draw(sdlRenderer);
    btnTwoTriangles->draw(sdlRenderer);

    // 使用自定义渲染器绘制几何体
    renderer->beginFrame();

    // 视口设置为右侧80%区域
    int viewportX = screenWidth * 0.2;
    int viewportWidth = screenWidth - viewportX;
    renderer->setViewport(viewportX, 0, viewportWidth, screenHeight);

    // 根据按钮状态绘制图形
    if (showBlueTriangle) {
        renderer->drawTriangle({ 0.5f, 0.0f, 0.0f }, Vec3f(0, 0, 1)); // 蓝色
    }
    if (showGreenTriangles) {
        renderer->drawTriangle({ -0.5f, -0.3f, 0.0f }, Vec3f(0, 1, 0)); // 绿1
        renderer->drawTriangle({ 0.5f, -0.3f, 0.0f }, Vec3f(0, 1, 0));  // 绿2
    }

    renderer->endFrame();

    // 交换缓冲区
    SDL_RenderPresent(sdlRenderer);
}

void Application::run() {
    while (true) {
        processEvents();
        update();
        render();
        SDL_Delay(16); // 约60FPS
    }
}