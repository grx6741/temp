#pragma once

#include <pch.h>

#include "Scene.hpp"

class SceneManager {
public:
    void updateFrame(float dt);
    void drawFrame();

    SceneManager();
    ~SceneManager();

    void addScene(SceneType name, Scene* scene);
private:
    std::unordered_map<SceneType, Scene*> m_Scenes;
    SceneType m_CurrentScene;
};
