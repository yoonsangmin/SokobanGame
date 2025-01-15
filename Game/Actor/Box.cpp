#include "Box.h"

Box::Box(const Vector2& position)
    : DrawableActor("b")
{
    // 위치 설정.
    this->position = position;

    // 색상 설정.
    color = Color::White;
}
