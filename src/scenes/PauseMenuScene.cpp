#include "PauseMenuScene.hpp"

PauseMenuScene::PauseMenuScene()
{

}

PauseMenuScene::~PauseMenuScene()
{
    
}

void PauseMenuScene::drawFrame()
{
    float centerx = GetScreenWidth() / 2.0f;
    float centery = GetScreenHeight() / 2.0f;

    float buttonWidth = 100;
    float buttonHeight = 20;

    float topleftx = centerx - buttonWidth / 2;
    float toplefty = centery - buttonHeight / 2;

    if (GuiButton({ topleftx, toplefty, buttonWidth, buttonHeight }, "Resume")) {
	m_NextScene = SceneType::INGAME;
    }
}

void PauseMenuScene::updateFrame(float dt)
{
    if (IsKeyPressed(KEY_ESCAPE)) {
	m_NextScene = SceneType::INGAME;
    }
}
