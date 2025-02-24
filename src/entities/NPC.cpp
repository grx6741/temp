#include "NPC.hpp"

#include "Components.hpp"
#include "Assets.hpp"
#include "AssetManager.hpp"

#include "net.hpp"

#define API_KEY "bdf3715fea0c4a16825d5f824a4aa606"

entt::entity NPC::create(entt::registry& registry, std::string name)
{
    const auto& npc = AssetManager::getInstance()->getAsset<json>(Assets::CONFIG_NPC)[name];
    entt::entity npc_id = registry.create();

    auto& npc_comp =  registry.emplace<Component::NPC>(npc_id);

    npc_comp.name = name;
    npc_comp.url = npc["url"];
    npc_comp.state = Component::NPC::State::IDLE;

    auto& transform = registry.emplace<Component::Transform>(npc_id, Vector2{0, 0});
    transform.pos = { 
	npc["start_position_x"],
	npc["start_position_y"],
    };

    auto& physics = registry.emplace<Component::Physics>(npc_id);
    physics.isGrounded = true;
    physics.size = { 
	npc["collider_width"],
	npc["collider_height"],
    };

    auto& sprite = registry.emplace<Component::Sprite>(npc_id, AssetManager::getInstance()->getAsset<SpriteSheet>(Assets::SPRITESHEET_NPC));

    sprite.currentState = 0;
    sprite.nextState = 0;

    sprite.spriteSheet
	 .addSequence(0)
	    .pos(Vector2 {0, 0})
	    .size(15, 17)
	    .count(4)
	    .fps(10)
	    .looping(true)
	    .padding(4, 4)
	    .spacing(9, 0)
	    .vertical(false)
	;

    return npc_id;
}

void NPC::think(entt::registry& registry, entt::entity npc_id, std::string player_text)
{
    Component::NPC& npc = registry.get<Component::NPC>(npc_id);
    npc.reply = "";

    net::RequestBody body = {};
    body.url = npc.url + "?loremType=business&type=words&number=" + std::to_string(GetRandomValue(1, 30));
    body.method = net::HTTPMethod::GET;
    body.userData = &npc;
    TraceLog(LOG_INFO, "NPC is sending request to: %s", body.url.c_str());
    body.headers = {
	"X-Api-Key", API_KEY
    };

    auto onSuccess = [](emscripten_fetch_t* fetch_data) {
	TraceLog(LOG_INFO, "SUCCESS");
	auto self = static_cast<Component::NPC*>(fetch_data->userData);

	self->reply = std::string(fetch_data->data, fetch_data->numBytes);

	TraceLog(LOG_INFO, "Quote: %s", self->reply.c_str());
	emscripten_fetch_close(fetch_data);
    };

    auto onError = [](emscripten_fetch_t* fetch_data) {
	auto self = static_cast<Component::NPC*>(fetch_data->userData);

	TraceLog(LOG_ERROR, "Request failed from %s : [%d] %s", fetch_data->url, fetch_data->status, fetch_data->statusText);
	emscripten_fetch_close(fetch_data);
    };

    auto onProgress = [](emscripten_fetch_t* fetch_data) {
	TraceLog(LOG_INFO, "Downloading from %s:%d", fetch_data->url, fetch_data->status);
    };

    net::request(body, onSuccess, onError, onProgress);
}
