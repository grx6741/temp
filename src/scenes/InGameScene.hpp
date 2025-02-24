#include "Scene.hpp"

#include "InputBox.hpp"

class InGameScene : public Scene {
public:
    InGameScene();
    ~InGameScene();
    virtual void drawFrame();
    virtual void updateFrame(float dt);

private:
    void updatePhysicEntites(float dt);
    void updateSpriteEntites(float dt);
    void updateNPCs(float dt);
    void updateCamera(float dt);

    void updateConversation();
    void drawConversation();
    void drawTextInBox(Font font, const char* text, Rectangle box, float fontSize, Color color);

private:
    entt::entity m_Player;

    struct {
	std::vector<entt::entity> m_Levels;
	size_t m_CurrentLevel;
    };

    Camera2D m_Camera;
    float m_Gravity;
    Font& m_TypingFont;

    enum class SceneState {
	NOT_IN_CONVERSATION,
	IN_CONVERSATION,
    } m_State;

    struct {
	/* Current NPC in Conversation */
	entt::entity npc;
	// Holds the text entered by the player
	InputBox inputBox;
	std::string reply;
    } m_Conversation;
};
