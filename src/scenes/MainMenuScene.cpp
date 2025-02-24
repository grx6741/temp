#include "MainMenuScene.hpp"

#define GAME_KEY "dev_93c481864de44fdcb2772e4a4c2ff425"

void MainMenuScene::drawFrame()
{
    float centerx = GetScreenWidth() / 2.0f;
    float centery = GetScreenHeight() / 2.0f;
    float gap = 20;

    float buttonWidth = 150;
    float buttonHeight = 30;	    

    float topleftx = centerx - buttonWidth / 2;
    float toplefty = centery - buttonHeight / 2 - 0.1 * GetScreenHeight();

    if (GuiButton({ topleftx, toplefty, buttonWidth, buttonHeight }, "Login as Guest")) {
	m_NextScene = SceneType::GUEST_LOGIN_MENU;
    }

    float toplefty2 = toplefty + buttonHeight + gap;

    if (GuiButton({ topleftx, toplefty2, buttonWidth, buttonHeight }, "Login with Email")) {
	// TODO
	m_NextScene = SceneType::INGAME;
    }
}

void MainMenuScene::updateFrame(float dt)
{

}

MainMenuScene::MainMenuScene()
{

}

MainMenuScene::~MainMenuScene()
{
    
}
