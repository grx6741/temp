#pragma once

#include "pch.h"

struct SpriteSheetSequence {
    Vector2 pos;        // Position in the spritesheet
    uint32_t width;     // Width of each frame in the sequence
    uint32_t height;    // Height of each frame in the sequence
    uint32_t count;     // Number of frames in the sequence
    uint32_t fps;       // Frames per second
    bool is_looping;    // Whether the animation loops
    bool is_vertical;   // Whether frames are arranged vertically

    // New attributes for padding and spacing
    uint32_t padding_left = 0;
    uint32_t padding_top = 0;
    uint32_t spacing_horizontal = 0;
    uint32_t spacing_vertical = 0;
};

class SpriteSheet {
public:
    SpriteSheet(const char* path);
    ~SpriteSheet();

    Rectangle getRect(int sequence, double time, bool *reached_last_frame = nullptr) const;
    bool draw(Vector2 pos, int sequence, double time, bool flip) const;

    // Builder pattern
    SpriteSheet& addSequence(int num);
    SpriteSheet& pos(Vector2 _pos);
    SpriteSheet& size(uint32_t _width, uint32_t _height);
    SpriteSheet& count(uint32_t _count);
    SpriteSheet& fps(uint32_t _fps);
    SpriteSheet& looping(bool _is_looping);
    SpriteSheet& vertical(bool _is_vertical);
    SpriteSheet& padding(uint32_t left, uint32_t top);
    SpriteSheet& spacing(uint32_t horizontal, uint32_t vertical);

private:
    Texture m_Texture;
    std::unordered_map<int, SpriteSheetSequence> m_SequenceMap;
    int m_LastCreatedSequence;
};
