#include "Components.hpp"

#include "InGameScene.hpp"

#include "Level.hpp"
#include "Player.hpp"
#include "NPC.hpp"

#include "AssetManager.hpp"
#include "Assets.hpp"

InGameScene::InGameScene() :
    m_Camera{0},
    m_Gravity(900.0),
    m_State(SceneState::NOT_IN_CONVERSATION),
    m_CurrentLevel(-1),
    m_TypingFont(AssetManager::getInstance()->getAsset<Font>(Assets::TYPING_FONT))
    // m_Level("assets/level/map.json", "assets/level/spritesheet.png")
{
    m_Levels.push_back(Level::create(m_Registry, "LEVEL1"));
    m_CurrentLevel = 0;

    m_Player = Player::create(m_Registry);
    NPC::create(m_Registry, "greg");

    const Vector2& player_pos = m_Registry.get<Component::Transform>(m_Player).pos;
    const Vector2& player_size = m_Registry.get<Component::Physics>(m_Player).size;

    m_Camera.target = Vector2Add(player_pos, Vector2Scale(player_size, 0.5));
    m_Camera.offset = Vector2 { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    m_Camera.rotation = 0.0f;
    m_Camera.zoom = 3.0f;
}

InGameScene::~InGameScene()
{

}

void InGameScene::drawFrame()
{
    BeginMode2D(m_Camera); {
		const auto& level = m_Registry.get<Component::Level>(m_Levels[m_CurrentLevel]);
		for (const auto layer_id : level.layers) {
			const auto& layer = m_Registry.get<Component::Layer>(layer_id);

			Layer::draw(m_Registry, m_Levels[m_CurrentLevel], layer_id);

			if (layer.name == "tiles") {
			// Draw all entites with a Sprite Component
			m_Registry.view<Component::Transform, Component::Sprite>()
				.each([this](entt::entity eID, const Component::Transform& transform, Component::Sprite& sprite) {
					// Draw a "Interact" label when player gets close to an NPC
					Component::NPC* npc_data = m_Registry.try_get<Component::NPC>(eID);
					if (npc_data) {
						if (npc_data->state == Component::NPC::State::IN_RANGE_TO_PLAYER) {
						const Vector2& npc_pos = transform.pos;
						const Vector2& npc_size = m_Registry.get<Component::Physics>(eID).size;

						DrawRectangleV(npc_pos, npc_size, RED);
						}
					}

					double time_since_last_state_changed = GetTime() - sprite.lastTimeWhenStateChanged;
					sprite.spriteSheet.draw(transform.pos, 
								sprite.currentState, 
								time_since_last_state_changed, 
								sprite.facingLeft);
				});
			}
		}
    } EndMode2D();

    // UI should be drawn outside (relative to the Screen)
    switch (m_State) {
	case SceneState::IN_CONVERSATION:
	    drawConversation();
	    break;
	case SceneState::NOT_IN_CONVERSATION:
	    break;
    }
}

void InGameScene::drawConversation()
{
    const char* text = m_Conversation.inputBox.get();

    Rectangle box_to_type = {
        static_cast<float>(0.05 * GetScreenWidth()),
        static_cast<float>(0.65 * GetScreenHeight()),
        static_cast<float>(0.9 * GetScreenWidth()),
        static_cast<float>(0.3 * GetScreenHeight())
    };

    Rectangle reply_box = {
        static_cast<float>(0.05 * GetScreenWidth()),
        static_cast<float>(0.05 * GetScreenHeight()),
        static_cast<float>(0.9 * GetScreenWidth()),
        static_cast<float>(0.3 * GetScreenHeight())
    };

    DrawRectangleRec(box_to_type, RED);
    DrawRectangleRec(reply_box, RED);

    drawTextInBox(m_TypingFont, text, box_to_type, 24, WHITE);
    drawTextInBox(m_TypingFont, m_Conversation.reply.c_str(), reply_box, 24, WHITE);

    if (m_Registry.get<Component::NPC>(m_Conversation.npc).state == Component::NPC::State::THINKING) {
        DrawCircle(reply_box.x + reply_box.width * 0.5, reply_box.y + reply_box.height * 0.5, 30, GREEN);
    }
}

void InGameScene::updateConversation()
{
    int charPressed = GetCharPressed(); // is a queue. will return 0 when queue is empty

    while (charPressed) { 
	m_Conversation.inputBox.insert(charPressed);
	charPressed = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) m_Conversation.inputBox.backspace();
}

void InGameScene::updateFrame(float dt)
{
    switch (m_State) {
	case SceneState::IN_CONVERSATION:
	    updateConversation();
	    break;
	case SceneState::NOT_IN_CONVERSATION:
	    Player::update(m_Registry);
	    if (IsKeyPressed(KEY_ESCAPE)) {
		m_NextScene = SceneType::PAUSE_MENU;
	    }
	    break;
    }

    updateNPCs(dt);

    updatePhysicEntites(dt);

    updateSpriteEntites(dt);

    updateCamera(dt);
}

void InGameScene::updatePhysicEntites(float dt)
{
    // Update All physics entities
    m_Registry.view<Component::Transform, Component::Physics>()
	.each([this, dt](Component::Transform& transform, Component::Physics& physics) {
	    Vector2& pos = transform.pos;
	    Vector2& vel = physics.vel;
	    const Vector2& size = physics.size;

	    pos.x += vel.x * dt;

	    Rectangle player_rect = { pos.x, pos.y, size.x, size.y };
	    const auto& collided_rects_x = Level::getCollidingRects(m_Registry, player_rect);

	    for (const auto& rect : collided_rects_x) {
		if (vel.x > 0) {
		    pos.x = rect.x - player_rect.width;
		} else if (vel.x < 0) {
		    pos.x = rect.x + rect.width;
		}
		vel.x = 0;
	    }

	    pos.y += vel.y * dt;

	    player_rect = { pos.x, pos.y, size.x, size.y };
	    const auto& collided_rects_y = Level::getCollidingRects(m_Registry, player_rect);

	    for (const auto& rect : collided_rects_y) {
		if (vel.y > 0) {
		    pos.y = rect.y - player_rect.height;
		    physics.isGrounded = true;
		} else if (vel.y < 0) {
		    pos.y = rect.y + rect.height;
		}
		vel.y = 0;
	    }

	    vel.y += m_Gravity * dt;
	});
}

void InGameScene::updateSpriteEntites(float dt)
{
    m_Registry.view<Component::Transform, Component::Sprite>()
	.each([this](const Component::Transform& transform, Component::Sprite& sprite) {
	    if (sprite.nextState != sprite.currentState) {
		sprite.currentState = sprite.nextState;
		sprite.lastTimeWhenStateChanged = GetTime();
	    }
	});
}


void InGameScene::updateNPCs(float dt)
{
    m_Registry.view<Component::NPC>()
	.each([this](entt::entity npc, Component::NPC& npc_data) {
		const Vector2& npc_pos = m_Registry.get<Component::Transform>(npc).pos;
		const Vector2& npc_size = m_Registry.get<Component::Physics>(npc).size;

		const Vector2& player_pos = m_Registry.get<Component::Transform>(m_Player).pos;
		const Vector2& player_size = m_Registry.get<Component::Physics>(m_Player).size;


		const Vector2 npc_center = Vector2Add(npc_pos, Vector2Scale(npc_size, 0.5));
		const Vector2 player_center = Vector2Add(player_pos, Vector2Scale(player_size, 0.5));

		const float distance = Vector2Distance(npc_center, player_center);

		switch (npc_data.state) {
		    case Component::NPC::State::IDLE:
			if (distance < 50) {
			    npc_data.state = Component::NPC::State::IN_RANGE_TO_PLAYER;
			}
			break;

		    case Component::NPC::State::IN_RANGE_TO_PLAYER:
			if (distance > 50) {
			    npc_data.state = Component::NPC::State::IDLE;
			}
			if (IsKeyPressed(KEY_E)) {
			    m_State = SceneState::IN_CONVERSATION;
			    m_Conversation.npc = npc;
			    npc_data.state = Component::NPC::State::LISTENING_TO_PLAYER;
			    npc_data.reply = "";
			}
			break;

		    case Component::NPC::State::LISTENING_TO_PLAYER:
			if (IsKeyPressed(KEY_ENTER)) {
			    NPC::think(m_Registry, m_Conversation.npc, m_Conversation.inputBox.get());
			    m_Conversation.reply = "";
			    m_Conversation.inputBox.clear();
			    npc_data.state = Component::NPC::State::THINKING;
			}
			if (IsKeyPressed(KEY_ESCAPE)) {
			    m_State = SceneState::NOT_IN_CONVERSATION;
			    m_Conversation.reply = "";
			    m_Conversation.inputBox.clear();
			}
			break;

		    case Component::NPC::State::THINKING:
			if (npc_data.reply != "") {
			    npc_data.state = Component::NPC::State::REPLYING;
			}
			break;

		    case Component::NPC::State::REPLYING:
			// Message is ready
			// Add one letter per frame, for animation
			static int i = 0;
			if (i < npc_data.reply.size()) {
			    m_Conversation.reply += npc_data.reply[i++];
			} else {
			    i = 0;
			    npc_data.state = Component::NPC::State::LISTENING_TO_PLAYER;
			}
			break;
		}
	    });
}

void InGameScene::updateCamera(float dt)
{
    const Vector2& player_pos = m_Registry.get<Component::Transform>(m_Player).pos;
    const Vector2& player_size = m_Registry.get<Component::Physics>(m_Player).size;

    float cameraSmoothness = 0.1f; // Adjust this for more/less smoothness
    m_Camera.target.x += (std::floor(player_pos.x + player_size.x / 2.0f) - m_Camera.target.x) * cameraSmoothness;
    m_Camera.target.y += (std::floor(player_pos.y + player_size.y / 2.0f) - m_Camera.target.y) * cameraSmoothness;
}

// Draws the text in a box, with word wrapping
void InGameScene::drawTextInBox(Font font, const char* text, Rectangle box, float fontSize, Color color)
{
    float lineSpacing = 1.0f;
    float spaceWidth = MeasureTextEx(font, " ", fontSize, lineSpacing).x;

    std::string word = "";
    std::string line = "";
    Vector2 position = { box.x + 0.05f * box.width, box.y + 0.05f * box.height };

    for (int i = 0; text[i] != '\0'; ++i) {
		if (text[i] == ' ' || text[i] == '\n') {
			float wordWidth = MeasureTextEx(font, (line + word).c_str(), fontSize, lineSpacing).x;
			if (wordWidth > box.width * 0.9f) {
				DrawTextEx(font, line.c_str(), position, fontSize, lineSpacing, color);
				position.y += fontSize * 1.5f;
				line = word + " ";
			} else {
				line += word + " ";
			}
			word = "";
		} else {
			word += text[i];
		}
    }

    if (!word.empty()) {
		float wordWidth = MeasureTextEx(font, (line + word).c_str(), fontSize, lineSpacing).x;
		if (wordWidth > box.width * 0.9f) {
			DrawTextEx(font, line.c_str(), position, fontSize, lineSpacing, color);
			position.y += fontSize * 1.5f;
			line = word;
		} else {
			line += word;
		}
    }

    if (!line.empty()) {
		DrawTextEx(font, line.c_str(), position, fontSize, lineSpacing, color);
    }
}
