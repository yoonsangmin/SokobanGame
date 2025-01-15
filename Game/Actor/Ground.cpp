#include "Ground.h"

Ground::Ground(const Vector2& position)
    : DrawableActor(".")
{
    // 위치 설정.
    this->position = position;

    // 색상 지정.
    color = Color::Red;
}