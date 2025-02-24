#include <pch.h>

#include "Game.hpp"

static Game* s_Game;

void UpdateDrawFrame() {
    s_Game->updateFrame(GetFrameTime());

    BeginDrawing(); {

	ClearBackground(BLACK);

	DrawFPS(0, 0);

	s_Game->drawFrame();

    } EndDrawing();
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Or any other flags you're using
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    s_Game = new Game();

    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);

    CloseWindow();

    return 0;
}
