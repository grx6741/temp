#include <pch.h>

#include "Game.hpp"
#include "AssetManager.hpp"
#include "SpriteSheet.hpp"
#include "Assets.hpp"

#include "PauseMenuScene.hpp"
#include "MainMenuScene.hpp"
#include "InGameScene.hpp"
#include "GuestLoginScene.hpp"

#include "../styles/sunny.h"

Game::Game()
{
    GuiLoadStyleSunny();

    AssetManager::getInstance()->addAsset<SpriteSheet>(Assets::SPRITESHEET_PLAYER, "assets/dino.png");
    AssetManager::getInstance()->addAsset<json>(Assets::CONFIG_PLAYER, "assets/config/player.json");

    AssetManager::getInstance()->addAsset<SpriteSheet>(Assets::SPRITESHEET_NPC, "assets/dino2.png");
    AssetManager::getInstance()->addAsset<json>(Assets::CONFIG_NPC, "assets/config/NPCs.json");

    AssetManager::getInstance()->addAsset<json>(Assets::LEVELS_DATA, "assets/config/level.json");

    AssetManager::getInstance()->addAsset<Font>(Assets::TYPING_FONT, "assets/font.ttf");

    m_SceneManager.addScene(SceneType::PAUSE_MENU, new PauseMenuScene());
    m_SceneManager.addScene(SceneType::GUEST_LOGIN_MENU , new GuestLoginScene());
    m_SceneManager.addScene(SceneType::MAIN_MENU, new MainMenuScene());
    m_SceneManager.addScene(SceneType::INGAME, new InGameScene());
}

Game::~Game()
{

}

void Game::drawFrame()
{
    m_SceneManager.drawFrame();
}

void Game::updateFrame(float dt)
{
    m_SceneManager.updateFrame(dt);
}
