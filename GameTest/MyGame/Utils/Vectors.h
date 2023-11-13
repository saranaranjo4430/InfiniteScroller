#pragma once

class Vector2D
{
public:
    Vector2D() : x(0.f), y(0.f) {};
    Vector2D(float _x, float _y) : x(_x), y(_y) {};

    float x;
    float y;
};