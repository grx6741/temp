#pragma once

#include "SceneManager.hpp"

class Game {
    public:
	Game();
	~Game();

	void drawFrame();
	void updateFrame(float dt);

    private:
	SceneManager m_SceneManager;
};
