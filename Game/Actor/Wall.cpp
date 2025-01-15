#include "Wall.h"

Wall::Wall(const Vector2& position)
    : DrawableActor("1")
{
    // 위치 설정.
    this->position = position;

    // 색상 설정.
    color = Color::Green;
}
