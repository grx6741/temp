#pragma once

#include <pch.h>

class Player {
private:
    static entt::entity m_Player;
    enum class PlayerState {
	IDLE,
	WALKING
    };

public:
    static entt::entity create(entt::registry& registry);
    static void update(entt::registry& registry);
};
