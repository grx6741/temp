#pragma once

#include "SpriteSheet.hpp"

namespace Component {

    struct Transform {
	Vector2 pos;

	Transform() : pos{0} {}
	Transform(Vector2 pos) : pos{pos} {}

	operator Vector2& () { return pos; }
	operator const Vector2& () const { return pos; }
    };

    struct Physics {
	Vector2 vel;
	Vector2 size;
	bool isGrounded;

	Physics() :
	    vel{0}, size{0}, isGrounded(false) {}
    };

    struct Sprite {
	SpriteSheet& spriteSheet;
	bool facingLeft;

	uint32_t currentState;
	uint32_t nextState;
	double lastTimeWhenStateChanged;

	Sprite(SpriteSheet& sp) : spriteSheet(sp) {}
    };

    struct Player {
	float speed;
	float jumpForce;
    };

    struct NPC {
	std::string name;

	std::string url;
	std::string reply;

	enum class State {
	    IDLE = 0,
	    IN_RANGE_TO_PLAYER,
	    THINKING, // getting reply from backend
	    REPLYING, // During Animating the text got from server
	    LISTENING_TO_PLAYER,
	} state;
    };

    struct Layer {
	std::string name;
	bool is_collidable;
	std::vector<std::vector<int>> tileIDs; // 2D grid of tile indices

	Layer(size_t map_width, size_t map_height) {
	    tileIDs = std::vector<std::vector<int>>(map_height, std::vector<int>(map_width, -1));
	}
    };

    struct Level {
	int tileSize;
	int width;
	int height;
	Texture tileSet;

	std::vector<entt::entity> layers;
	std::unordered_map<int, Vector2> tileLookup; // ID to tileset position lookup
    };
}
