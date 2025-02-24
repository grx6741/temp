#include "Scene.hpp"

class PauseMenuScene : public Scene {
public:
    PauseMenuScene();
    ~PauseMenuScene();
    virtual void drawFrame();
    virtual void updateFrame(float dt);
};
