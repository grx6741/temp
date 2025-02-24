#include <fstream>

#include "pch.h"

#include "Level.hpp"
#include "Components.hpp"

#include "AssetManager.hpp"
#include "Assets.hpp"

entt::entity Level::create(entt::registry& registry, const char* name)
{
    const auto& level_config = AssetManager::getInstance()->getAsset<json>(Assets::LEVELS_DATA)[name];
    entt::entity level_id = registry.create();

    std::ifstream level_input_stream(level_config["map"]);
    json level_data;
    level_input_stream >> level_data;

    auto& level = registry.emplace<Component::Level>(level_id);
    level.tileSize = level_data["tileSize"];
    level.width = level_data["mapWidth"];
    level.height = level_data["mapHeight"];
    level.tileSet = LoadTexture(std::string(level_config["tileSet"]).c_str());

    for (const auto& layer_data : level_data["layers"]) {
        entt::entity layer_id = Layer::create(registry, layer_data, level);
        level.layers.push_back(layer_id);
    }

    return level_id;
}

entt::entity Layer::create(entt::registry& registry, const json& layer_data, Component::Level& level)
{
    entt::entity layer_id = registry.create();
    auto& layer = registry.emplace<Component::Layer>(layer_id, level.width, level.height);

    layer.name = layer_data["name"];
    layer.is_collidable = layer_data.value("collider", false);

    int sheetCols = level.tileSet.width / level.tileSize;

    for (const auto& tile_data : layer_data["tiles"]) {
        int id = std::stoi(tile_data["id"].get<std::string>());
        int x = tile_data["x"];
        int y = tile_data["y"];
        layer.tileIDs[y][x] = id;

	if (level.tileLookup.find(id) == level.tileLookup.end()) {
	    level.tileLookup[id] = {
		static_cast<float>((id % sheetCols) * level.tileSize),
		static_cast<float>(std::floor(id / (float)sheetCols) * level.tileSize)
	    };
	}
    }

    return layer_id;
}

void Layer::draw(entt::registry& registry, entt::entity level_id, entt::entity layer_id)
{
    if (!registry.valid(layer_id) || !registry.all_of<Component::Layer>(layer_id)) return;

    auto& layer = registry.get<Component::Layer>(layer_id);

    if (layer.tileIDs.empty()) return; // Ensure layer has tiles

    if (!registry.valid(level_id)) return;

    auto& level = registry.get<Component::Level>(level_id);
    int tileSize = level.tileSize;
    int sheetCols = level.tileSet.width / tileSize;

    for (int y = 0; y < layer.tileIDs.size(); ++y) {
        for (int x = 0; x < layer.tileIDs[y].size(); ++x) {
            int tileID = layer.tileIDs[y][x];
            if (tileID == -1) continue;

            Vector2 tile_pos_in_spritesheet = level.tileLookup.at(tileID);
            Rectangle tile_rect = {
		std::floor(tile_pos_in_spritesheet.x),
                std::floor(tile_pos_in_spritesheet.y),
                std::floor(static_cast<float>(tileSize)),
                std::floor(static_cast<float>(tileSize))
            };

            Vector2 pos = { 
		std::floor(x * static_cast<float>(tileSize)), 
		std::floor(y * static_cast<float>(tileSize))
	    };
            DrawTextureRec(level.tileSet, tile_rect, pos, WHITE);
        }
    }
}

std::vector<Rectangle> Level::getCollidingRects(entt::registry& registry, const Rectangle& player)
{
    std::vector<Rectangle> colliding_rects;

    // Fetch Level entity to get map size & tile size
    entt::entity level_id = registry.view<Component::Level>().front();
    if (!registry.valid(level_id)) return colliding_rects;

    auto& level = registry.get<Component::Level>(level_id);
    int tileSize = level.tileSize;

    // Loop through all layers with collision enabled
    for (auto entity : registry.view<Component::Layer>()) {
        auto& layer = registry.get<Component::Layer>(entity);
        if (!layer.is_collidable) continue;

        // auto rects = getNeighbourRects(player, tileSize);
	Vector2 player_pos = { 
	    static_cast<float>(player.x + player.width * 0.5), 
	    static_cast<float>(player.y + player.height * 0.5)
	};
        auto rects = getNeighbourRects(registry, player_pos);
        for (const Rectangle& rect : rects) {
            int x = static_cast<int>(rect.x / tileSize);
            int y = static_cast<int>(rect.y / tileSize);

            if (!(0 <= x && x < level.width)) continue;
            if (!(0 <= y && y < level.height)) continue;
            if (layer.tileIDs[y][x] == -1) continue;

            if (CheckCollisionRecs(rect, player)) {
                colliding_rects.push_back(rect);
            }
        }
    }
    return colliding_rects;
}

std::vector<Rectangle> Level::getNeighbourRects(entt::registry& registry, const Vector2& player)
{
    std::vector<Rectangle> rects;

    // Get Level entity to retrieve tile size
    entt::entity level_id = registry.view<Component::Level>().front();
    if (!registry.valid(level_id)) return rects;

    auto& level = registry.get<Component::Level>(level_id);
    int tileSize = level.tileSize;

    // Offsets for adjacent tiles
    std::vector<Vector2> offsets = {
        {-1,  0}, {-1, -1}, { 0, -1}, { 1, -1},
        { 1,  0}, { 1,  1}, { 0,  1}, {-1,  1}
    };

    Vector2 tilePosition = {
        std::floor(player.x / tileSize),
        std::floor(player.y / tileSize)
    };

    // Generate neighboring tile rectangles
    for (const Vector2& offset : offsets) {
        Vector2 tile = Vector2Add(offset, tilePosition);
        rects.push_back(Rectangle{ 
            tile.x * tileSize, 
            tile.y * tileSize, 
            static_cast<float>(tileSize), 
            static_cast<float>(tileSize)
        });
    }

    return rects;
}
