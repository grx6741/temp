#include "SpriteSheet.hpp"

Rectangle SpriteSheet::getRect(int sequence, double time, bool *reached_last_frame) const
{
    const SpriteSheetSequence& curr = m_SequenceMap.at(sequence);
    int frame_index = curr.fps * time;

    if (reached_last_frame != nullptr) {
        *reached_last_frame = !curr.is_looping && frame_index >= curr.count;
    }

    frame_index %= curr.count;

    // Adjust for vertical or horizontal layout
    Rectangle frame_rect;
    if (curr.is_vertical) {
        frame_rect = {
            curr.pos.x + curr.padding_left,
            curr.pos.y + curr.padding_top + (curr.height + curr.spacing_vertical) * frame_index,
            static_cast<float>(curr.width),
            static_cast<float>(curr.height)
        };
    } else {
        frame_rect = {
            curr.pos.x + curr.padding_left + (curr.width + curr.spacing_horizontal) * frame_index,
            curr.pos.y + curr.padding_top,
            static_cast<float>(curr.width),
            static_cast<float>(curr.height)
        };
    }

    return frame_rect;
}

bool SpriteSheet::draw(Vector2 pos, int sequence, double time, bool flip) const
{
    Rectangle rect = getRect(sequence, time);
    rect.width *= (flip) ? -1 : 1; // Flip horizontally if needed
    DrawTextureRec(m_Texture, rect, pos, WHITE);
    return false;
}

SpriteSheet::SpriteSheet(const char* path)
{
    m_Texture = LoadTexture(path);
}

SpriteSheet::~SpriteSheet()
{
    UnloadTexture(m_Texture);
}

SpriteSheet& SpriteSheet::addSequence(int num)
{
    m_LastCreatedSequence = num;
    m_SequenceMap[m_LastCreatedSequence] = {0};
    return *this;
}

SpriteSheet& SpriteSheet::pos(Vector2 _pos)
{
    m_SequenceMap[m_LastCreatedSequence].pos = _pos;
    return *this;
}

SpriteSheet& SpriteSheet::size(uint32_t _width, uint32_t _height)
{
    m_SequenceMap[m_LastCreatedSequence].width = _width;
    m_SequenceMap[m_LastCreatedSequence].height = _height;
    return *this;
}

SpriteSheet& SpriteSheet::count(uint32_t _count)
{
    m_SequenceMap[m_LastCreatedSequence].count = _count;
    return *this;
}

SpriteSheet& SpriteSheet::fps(uint32_t _fps)
{
    m_SequenceMap[m_LastCreatedSequence].fps = _fps;
    return *this;
}

SpriteSheet& SpriteSheet::looping(bool _is_looping)
{
    m_SequenceMap[m_LastCreatedSequence].is_looping = _is_looping;
    return *this;
}

SpriteSheet& SpriteSheet::vertical(bool _is_vertical)
{
    m_SequenceMap[m_LastCreatedSequence].is_vertical = _is_vertical;
    return *this;
}

SpriteSheet& SpriteSheet::padding(uint32_t left, uint32_t top)
{
    m_SequenceMap[m_LastCreatedSequence].padding_left = left;
    m_SequenceMap[m_LastCreatedSequence].padding_top = top;
    return *this;
}

SpriteSheet& SpriteSheet::spacing(uint32_t horizontal, uint32_t vertical)
{
    m_SequenceMap[m_LastCreatedSequence].spacing_horizontal = horizontal;
    m_SequenceMap[m_LastCreatedSequence].spacing_vertical = vertical;
    return *this;
}
