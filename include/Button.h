#pragma once
#include <SDL.h>
#include <string>

// ��ť�ؼ���
class Button {
public:
    Button(int x, int y, int w, int h, const std::string& text);

    void handleEvent(SDL_Event& e);
    void draw(SDL_Renderer* renderer);

    bool isClicked() const { return clicked; }
    void resetState() { clicked = false; }

private:
    SDL_Rect rect;          // ��ťλ�úͳߴ�
    std::string label;      // ��ť�ı�
    bool hovered = false;   // ��ͣ״̬
    bool clicked = false;   // ���״̬
};