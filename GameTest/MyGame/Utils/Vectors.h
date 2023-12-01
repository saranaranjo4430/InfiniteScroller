#pragma once

#include "GameViewport.h"
#include "Utils.h"

class Circle2D;

class Vector2D
{
public:
    Vector2D() : x(0.f), y(0.f) {};
    Vector2D(float _x, float _y) : x(_x), y(_y) {};

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    void operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
    }

    Vector2D operator*(const float factor) const {
        return Vector2D(x * factor, y * factor);
    }

    void operator*=(const Vector2D& other) {
        x *= other.x;
        y *= other.y;
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    void operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
    }

    Vector2D operator-() {
        return Vector2D(-x, -y);
    }

    float GetNorm() const;
    float GetNorm2() const;
    
    void Normalize(float _len = 1.f);
    void Rotate(float _degrees);

    float Distance(const Vector2D& _other) const;
    float Dot(const Vector2D& _other) const;
    float Angle(const Vector2D& _other) const;

    float x;
    float y;
};

class Rect2D
{
public:
    Rect2D() {};
    Rect2D(Vector2D& _center, float _width, float _height);

    void Move(float _x, float _y);
    void Rotate(float _degrees);
    void Scale(float _scale);

    Vector2D GetCenter() const { return m_Center; }
    float GetRotation() const { return m_Angle; }
    float GetScale() const { return m_Scale; }

    void Draw(const Utils::Color& _color) const;
    
    bool Overlap(const Rect2D& _other) const;
    bool Overlap(const Circle2D& _circle) const;

protected:
    void ComputePoints();

private:
    Vector2D m_Center;
    float m_Width = 0.f;
    float m_Height = 0.f;
    float m_Angle = 0.f;
    float m_Scale = 1.f;

    Vector2D m_Points[4];
};

class Square2D : public Rect2D
{
public:
    Square2D() {};
    Square2D(Vector2D& _center, float _size);
};

class Circle2D
{
public:
    Circle2D() {};
    Circle2D(Vector2D& _center, float _radius);

    void Move(float _x, float _y);

    Vector2D GetCenter() const { return m_Center; }
    float GetRadius() const { return m_Radius; }

    void Draw(const Utils::Color& _color, int sharpness = 10) const;

    bool Overlap(const Rect2D& _rect) const;
    bool Overlap(const Circle2D& _other) const;

private:
    Vector2D m_Center;
    float m_Radius = 0.f;
};