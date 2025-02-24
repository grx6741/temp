#pragma once

#include <pch.h>

class NPC {
public:
    static entt::entity create(entt::registry& registry, std::string name);
    static void think(entt::registry& registry, entt::entity npc_id, std::string player_text);
};
