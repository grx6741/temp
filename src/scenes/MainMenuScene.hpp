#include "Scene.hpp"

class MainMenuScene : public Scene {
public:
    MainMenuScene();
    ~MainMenuScene();
    virtual void drawFrame();
    virtual void updateFrame(float dt);

};
