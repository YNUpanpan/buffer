#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
class Window
{
private:
    // ���崰�ڵĿ�Ⱥ͸߶�
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // ���尴ť��λ�úʹ�С
    const int START_BUTTON_X = 50;
    const int START_BUTTON_Y = 200;
    const int BUTTON_WIDTH = 120;
    const int BUTTON_HEIGHT = 50;

    const int STOP_BUTTON_X = 50;
    const int STOP_BUTTON_Y = 500;

    // ��¼��ʼʱ��ͽ���ʱ��
    Uint32 startTime = 0;
    Uint32 endTime = 0;
    // ����Ƿ����ڼ�ʱ
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


// ��Ⱦ�ı�����
void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) { }

// ���������Ƿ��ڰ�ť������
bool isMouseInButton(int mouseX, int mouseY, int buttonX, int buttonY) {}

int main(int argc, char* argv[]) {
    // ��ʼ�� SDL ��
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // ��ʼ�� SDL_ttf ��
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // ��������
    SDL_Window* window = SDL_CreateWindow(
        "Timer Example",  // ���ڱ���
        SDL_WINDOWPOS_UNDEFINED,  // ���ڳ�ʼ X λ��
        SDL_WINDOWPOS_UNDEFINED,  // ���ڳ�ʼ Y λ��
        WINDOW_WIDTH,  // ���ڿ��
        WINDOW_HEIGHT,  // ���ڸ߶�
        SDL_WINDOW_SHOWN  // ������ʾ��־
    );
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // ������Ⱦ��
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,  // �����Ĵ���
        -1,  // ��Ⱦ����������-1 ��ʾѡ���һ�����õļ�����Ⱦ��
        SDL_RENDERER_ACCELERATED  // ������Ⱦ��־
    );
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // �������ļ�������ʹ�� 24 ������
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // �¼�ѭ����־
    bool quit = false;
    // �¼��ṹ��
    SDL_Event e;

    while (!quit) {
        // �����¼�
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                // ����û�����رմ��ڰ�ť���˳�ѭ��
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                // ��ȡ�����������
                SDL_GetMouseState(&mouseX, &mouseY);

                if (isMouseInButton(mouseX, mouseY, START_BUTTON_X, START_BUTTON_Y)) {
                    // ��������ʼ��ť
                    isTiming = true;  // ��ʼ��ʱ��־��Ϊ true
                    startTime = SDL_GetTicks();  // ��¼��ʼʱ��
                }
                else if (isMouseInButton(mouseX, mouseY, STOP_BUTTON_X, STOP_BUTTON_Y)) {
                    // ������ֹͣ��ť
                    if (isTiming) {
                        // ������ڼ�ʱ
                        endTime = SDL_GetTicks();  // ��¼����ʱ��
                        isTiming = false;  // ֹͣ��ʱ��־��Ϊ false
                    }
                }
            }
        }
        std::cout << 1;
        // ���������ñ�����ɫΪ��ɫ
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // ���ƿ�ʼ��ť
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // ���ð�ť��ɫΪ��ɫ
        SDL_Rect startButtonRect = { START_BUTTON_X, START_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
        SDL_RenderFillRect(renderer, &startButtonRect);
        // �ڿ�ʼ��ť����ʾ�ı�
        renderText(renderer, font, "begin", START_BUTTON_X + 20, START_BUTTON_Y + 15);

        // ����ֹͣ��ť
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // ���ð�ť��ɫΪ��ɫ
        SDL_Rect stopButtonRect = { STOP_BUTTON_X, STOP_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
        SDL_RenderFillRect(renderer, &stopButtonRect);
        // ��ֹͣ��ť����ʾ�ı�
        renderText(renderer, font, "end", STOP_BUTTON_X + 20, STOP_BUTTON_Y + 15);

        // ���㲢��ʾ������ʱ��
        if (!isTiming && startTime != 0) {
            Uint32 elapsedTime = endTime - startTime;
            double elapsedSeconds = static_cast<double>(elapsedTime) / 1000.0;
            std::ostringstream oss;
            oss << "time:" << elapsedSeconds << "s";
            renderText(renderer, font, oss.str(), 120, 50);
        }

        // ������Ļ
        SDL_RenderPresent(renderer);
    }

    // �ر�����
    TTF_CloseFont(font);
    // ������Ⱦ��
    SDL_DestroyRenderer(renderer);
    // ���ٴ���
    SDL_DestroyWindow(window);
    // �˳� SDL_ttf ��
    TTF_Quit();
    // �˳� SDL ��
    SDL_Quit();

    return 0;
}