#include "SceneManager.hpp"

void SceneManager::addScene(SceneType name, Scene* scene)
{
    m_Scenes[name] = scene;
    m_CurrentScene = name;
}

void SceneManager::updateFrame(float dt)
{
    Scene* curr = m_Scenes[m_CurrentScene];
    if (curr->getNextScene() != SceneType::NONE) {
	m_CurrentScene = curr->getNextScene();
	curr->setNextScene(SceneType::NONE);
    }

    curr->updateFrame(dt);
}

void SceneManager::drawFrame()
{
    m_Scenes[m_CurrentScene]->drawFrame();
}

SceneManager::SceneManager() :
    m_CurrentScene(SceneType::NONE)
{

}

SceneManager::~SceneManager()
{

}
