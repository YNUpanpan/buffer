#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
class Window
{
private:
    // 定义窗口的宽度和高度
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // 定义按钮的位置和大小
    const int START_BUTTON_X = 50;
    const int START_BUTTON_Y = 200;
    const int BUTTON_WIDTH = 120;
    const int BUTTON_HEIGHT = 50;

    const int STOP_BUTTON_X = 50;
    const int STOP_BUTTON_Y = 500;

    // 记录开始时间和结束时间
    Uint32 startTime = 0;
    Uint32 endTime = 0;
    // 标记是否正在计时
    bool isTiming = false;
public:
    Window();
    ~Window();

    int getWindowWidth() { return 1; };
    int getWindowHeight() { return 1; };
    bool setup() {};
};

Window::Window()
{
}

Window::~Window()
{
}


// 渲染文本函数
void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) { }

// 检查鼠标点击是否在按钮区域内
bool isMouseInButton(int mouseX, int mouseY, int buttonX, int buttonY) {}

int main(int argc, char* argv[]) {
    // 初始化 SDL 库
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // 初始化 SDL_ttf 库
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow(
        "Timer Example",  // 窗口标题
        SDL_WINDOWPOS_UNDEFINED,  // 窗口初始 X 位置
        SDL_WINDOWPOS_UNDEFINED,  // 窗口初始 Y 位置
        WINDOW_WIDTH,  // 窗口宽度
        WINDOW_HEIGHT,  // 窗口高度
        SDL_WINDOW_SHOWN  // 窗口显示标志
    );
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,  // 关联的窗口
        -1,  // 渲染驱动索引，-1 表示选择第一个可用的加速渲染器
        SDL_RENDERER_ACCELERATED  // 加速渲染标志
    );
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // 打开字体文件，这里使用 24 号字体
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // 事件循环标志
    bool quit = false;
    // 事件结构体
    SDL_Event e;

    while (!quit) {
        // 处理事件
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                // 如果用户点击关闭窗口按钮，退出循环
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                // 获取鼠标点击的坐标
                SDL_GetMouseState(&mouseX, &mouseY);

                if (isMouseInButton(mouseX, mouseY, START_BUTTON_X, START_BUTTON_Y)) {
                    // 如果点击开始按钮
                    isTiming = true;  // 开始计时标志置为 true
                    startTime = SDL_GetTicks();  // 记录开始时间
                }
                else if (isMouseInButton(mouseX, mouseY, STOP_BUTTON_X, STOP_BUTTON_Y)) {
                    // 如果点击停止按钮
                    if (isTiming) {
                        // 如果正在计时
                        endTime = SDL_GetTicks();  // 记录结束时间
                        isTiming = false;  // 停止计时标志置为 false
                    }
                }
            }
        }
        std::cout << 1;
        // 清屏，设置背景颜色为白色
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // 绘制开始按钮
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // 设置按钮颜色为绿色
        SDL_Rect startButtonRect = { START_BUTTON_X, START_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
        SDL_RenderFillRect(renderer, &startButtonRect);
        // 在开始按钮上显示文本
        renderText(renderer, font, "begin", START_BUTTON_X + 20, START_BUTTON_Y + 15);

        // 绘制停止按钮
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // 设置按钮颜色为红色
        SDL_Rect stopButtonRect = { STOP_BUTTON_X, STOP_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
        SDL_RenderFillRect(renderer, &stopButtonRect);
        // 在停止按钮上显示文本
        renderText(renderer, font, "end", STOP_BUTTON_X + 20, STOP_BUTTON_Y + 15);

        // 计算并显示经过的时间
        if (!isTiming && startTime != 0) {
            Uint32 elapsedTime = endTime - startTime;
            double elapsedSeconds = static_cast<double>(elapsedTime) / 1000.0;
            std::ostringstream oss;
            oss << "time:" << elapsedSeconds << "s";
            renderText(renderer, font, oss.str(), 120, 50);
        }

        // 更新屏幕
        SDL_RenderPresent(renderer);
    }

    // 关闭字体
    TTF_CloseFont(font);
    // 销毁渲染器
    SDL_DestroyRenderer(renderer);
    // 销毁窗口
    SDL_DestroyWindow(window);
    // 退出 SDL_ttf 库
    TTF_Quit();
    // 退出 SDL 库
    SDL_Quit();

    return 0;
}