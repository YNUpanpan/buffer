#include "Application.h"
#include"geometry.h"
Application::Application(int width, int height)
    : screenWidth(width), screenHeight(height) {

    // ��ʼ��SDL
    SDL_Init(SDL_INIT_VIDEO);

    // ��������
    window = SDL_CreateWindow("MVP Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN);

    // ����SDL��Ⱦ��������UIʹ�ã�
    sdlRenderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED);

    // ��ʼ���Զ�����Ⱦ����ʹ�ö�����ȾĿ�꣩
    renderer = new Render(width, height);

    // ������ť�����20%����
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

        // ����ť��������¼����괫�ݸ���ť��
        btnTriangle->handleEvent(e);
        btnTwoTriangles->handleEvent(e);
    }
}

void Application::update() {
    // ��ⰴť״̬
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
    // ��ջ�����
    SDL_SetRenderDrawColor(sdlRenderer, 45, 45, 45, 255);
    SDL_RenderClear(sdlRenderer);

    // ���ư�ť��ʹ��SDLԭ����Ⱦ��
    btnTriangle->draw(sdlRenderer);
    btnTwoTriangles->draw(sdlRenderer);

    // ʹ���Զ�����Ⱦ�����Ƽ�����
    renderer->beginFrame();

    // �ӿ�����Ϊ�Ҳ�80%����
    int viewportX = screenWidth * 0.2;
    int viewportWidth = screenWidth - viewportX;
    renderer->setViewport(viewportX, 0, viewportWidth, screenHeight);

    // ���ݰ�ť״̬����ͼ��
    if (showBlueTriangle) {
        renderer->drawTriangle({ 0.5f, 0.0f, 0.0f }, Vec3f(0, 0, 1)); // ��ɫ
    }
    if (showGreenTriangles) {
        renderer->drawTriangle({ -0.5f, -0.3f, 0.0f }, Vec3f(0, 1, 0)); // ��1
        renderer->drawTriangle({ 0.5f, -0.3f, 0.0f }, Vec3f(0, 1, 0));  // ��2
    }

    renderer->endFrame();

    // ����������
    SDL_RenderPresent(sdlRenderer);
}

void Application::run() {
    while (true) {
        processEvents();
        update();
        render();
        SDL_Delay(16); // Լ60FPS
    }
}