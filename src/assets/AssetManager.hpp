#pragma once

#include <pch.h>

#include "SpriteSheet.hpp"

#define MAX_ASSETS 1024

class AssetManager {
public:
    static AssetManager* getInstance();

    template<class T> void addAsset(uint32_t key, const char* path);
    template<class T> T& getAsset(uint32_t key) const;

private:
    static std::unordered_map<uint32_t, SpriteSheet> m_SpriteSheets;
    static std::unordered_map<uint32_t, json> m_Configs;
    static std::unordered_map<uint32_t, Font> m_Fonts;

    ~AssetManager();
};
