#include "AssetManager.hpp"
#include <fstream>

std::unordered_map<uint32_t, SpriteSheet> AssetManager::m_SpriteSheets;
std::unordered_map<uint32_t, json> AssetManager::m_Configs;
std::unordered_map<uint32_t, Font> AssetManager::m_Fonts;

template <> 
void AssetManager::addAsset<Font>(uint32_t key, const char* path)
{
    std::cout << "Adding asset: " << key << " Path: " << path << "\n";
    m_Fonts[key] = LoadFont(path);
}

template <> 
void AssetManager::addAsset<json>(uint32_t key, const char* path)
{
    std::cout << "Adding asset: " << key << " Path: " << path << "\n";
    std::ifstream f(path);
    json data;

    f >> data;
    m_Configs[key] = std::move(data);
}

template <> 
void AssetManager::addAsset<SpriteSheet>(uint32_t key, const char* path)
{
    std::cout << "Adding asset: " << key << " Path: " << path << "\n";
    m_SpriteSheets.emplace(key, path);
}

template <>
SpriteSheet& AssetManager::getAsset<SpriteSheet>(uint32_t key) const
{
    std::cout << "Getting SP asset: " << key << "\n";
    return m_SpriteSheets.at(key);
}

template <>
json& AssetManager::getAsset<json>(uint32_t key) const
{
    std::cout << "Getting JS asset: " << key << "\n";
    return m_Configs.at(key);
}

template <>
Font& AssetManager::getAsset<Font>(uint32_t key) const
{
    std::cout << "Getting Font asset: " << key << "\n";
    return m_Fonts.at(key);
}

AssetManager* AssetManager::getInstance()
{
    static AssetManager instance;
    return &instance;
}

AssetManager::~AssetManager()
{
    for (auto& font_pair : m_Fonts) {
	Font& font = font_pair.second;
	UnloadFont(font);
    }
}
