#include "Button.h"

Button::Button(int x, int y, int w, int h, const std::string& text)
    : rect{ x, y, w, h }, label(text) {
}

void Button::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // 检测鼠标是否在按钮内
        hovered = (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h);

        if (hovered && e.type == SDL_MOUSEBUTTONDOWN) {
            clicked = true;
        }
    }
}

void Button::draw(SDL_Renderer* renderer) {
    // 绘制背景
    SDL_SetRenderDrawColor(renderer,
        hovered ? 100 : 60,
        hovered ? 100 : 60,
        hovered ? 100 : 60, 255);
    SDL_RenderFillRect(renderer, &rect);

    // 绘制边框
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &rect);

    // 此处应添加文本渲染（示例省略SDL_ttf部分）
}