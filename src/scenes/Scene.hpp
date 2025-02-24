#pragma once

enum class SceneType {
    MAIN_MENU,
    GUEST_LOGIN_MENU,
    INGAME,
    PAUSE_MENU,
    NONE
};

class Scene {
public:
    Scene() : m_NextScene(SceneType::NONE) {}
    virtual void drawFrame() = 0;
    virtual void updateFrame(float dt) = 0;

    inline const SceneType& getNextScene() const { return m_NextScene; }
    inline void setNextScene(SceneType type) { m_NextScene = type; }

protected:
    SceneType m_NextScene;
    entt::registry m_Registry;
};
