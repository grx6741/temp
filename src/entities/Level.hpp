#include "Components.hpp"

#pragma once

class Level {
    public:
	static entt::entity create(entt::registry& registry, const char* name);
	static std::vector<Rectangle> getCollidingRects(entt::registry& registry, const Rectangle& player);

    private:
	static std::vector<Rectangle> getNeighbourRects(entt::registry& registry, const Vector2& player);
};

class Layer {
    public:
	static entt::entity create(entt::registry& registry, const json& layer_data, Component::Level& level);
	static void draw(entt::registry& registry, entt::entity level_id, entt::entity layer_id);
};
