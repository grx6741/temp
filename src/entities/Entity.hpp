#pragma once

#include "pch.h"

// Anything in the World
class Entity {
    public:
	const Vector2 getPos() const { return m_Pos; }

    private:
	Vector2 m_Pos;
};
