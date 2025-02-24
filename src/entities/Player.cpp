#include "Player.hpp"
#include "Components.hpp"
#include "AssetManager.hpp"
#include "Assets.hpp"

entt::entity Player::m_Player = entt::null;

entt::entity Player::create(entt::registry& registry)
{
    const auto& player_data = AssetManager::getInstance()->getAsset<json>(Assets::CONFIG_PLAYER);
    m_Player = registry.create();

    auto& transform = registry.emplace<Component::Transform>(m_Player, Vector2{0, 0});
    transform.pos = { 
	player_data["start_position_x"],
	player_data["start_position_y"],
    };

    auto& physics = registry.emplace<Component::Physics>(m_Player);
    physics.isGrounded = true;
    physics.size = { 
	player_data["collider_width"],
	player_data["collider_height"],
    };

    auto& sprite = registry.emplace<Component::Sprite>(m_Player, AssetManager::getInstance()->getAsset<SpriteSheet>(Assets::SPRITESHEET_PLAYER));

    sprite.currentState = (uint32_t)PlayerState::IDLE;
    sprite.nextState = (uint32_t)PlayerState::IDLE;

    sprite.spriteSheet
	 .addSequence((uint32_t)PlayerState::IDLE)
	    .pos(Vector2 {0, 0})
	    .size(15, 17)
	    .count(4)
	    .fps(10)
	    .looping(true)
	    .padding(4, 4)
	    .spacing(9, 0)
	    .vertical(false)
	.addSequence((uint32_t)PlayerState::WALKING)
	    .pos(Vector2 {24 * 4, 0})
	    .size(15, 17)
	    .count(4)
	    .fps(10)
	    .looping(true)
	    .padding(4, 4)
	    .spacing(9, 0)
	    .vertical(false)
	;

    auto& self = registry.emplace<Component::Player>(m_Player);

    self.speed = player_data["speed"];
    self.jumpForce = player_data["jump_force"];

    return m_Player;
}

void Player::update(entt::registry& registry)
{
    auto& physics = registry.get<Component::Physics>(m_Player);
    auto& sprite = registry.get<Component::Sprite>(m_Player);
    auto& self = registry.get<Component::Player>(m_Player);

    physics.vel.x = 0;

    if (IsKeyDown(KEY_A)) {
	physics.vel.x = -self.speed;
	sprite.facingLeft = true;
    }
    if (IsKeyDown(KEY_D)) {
	physics.vel.x = self.speed;
	sprite.facingLeft = false;
    }

    if (physics.vel.x != 0) {
	sprite.nextState = (uint32_t)PlayerState::WALKING;
    } else {
	sprite.nextState = (uint32_t)PlayerState::IDLE;
    }

    if (IsKeyDown(KEY_W) && physics.isGrounded) {
	physics.vel.y = -self.jumpForce;
	physics.isGrounded = false;
    }
}
