#pragma once
#include <SDL.h>
#include <string>

// 按钮控件类
class Button {
public:
    Button(int x, int y, int w, int h, const std::string& text);

    void handleEvent(SDL_Event& e);
    void draw(SDL_Renderer* renderer);

    bool isClicked() const { return clicked; }
    void resetState() { clicked = false; }

private:
    SDL_Rect rect;          // 按钮位置和尺寸
    std::string label;      // 按钮文本
    bool hovered = false;   // 悬停状态
    bool clicked = false;   // 点击状态
};