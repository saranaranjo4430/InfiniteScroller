#pragma once

#include "GameViewport.h"
#include "Utils.h"

class Ellipse2D;

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

    bool operator!=(const Vector2D& other) {
        return (x != other.x || y != other.y);
    }

    bool operator==(const Vector2D& other) {
        return (x == other.x && y == other.y);
    }

    float GetNorm() const;
    float GetNorm2() const;

    void Normalize(float _len = 1.f);
    void Rotate(float _degrees);

    float Distance(const Vector2D& _other) const;
    float Dot(const Vector2D& _other) const;
    float Angle(const Vector2D& _other) const;

    void Draw(const Vector2D& _center, const Utils::Color& _color = Utils::Color_White) const;

    float x;
    float y;
};

class Rect2D
{
public:
    Rect2D() {};
    Rect2D(const Vector2D& _center, float _width, float _height);

    void Move(const Vector2D& _pos);
    void Rotate(float _degrees);
    void Scale(float _scaleX, float _scaleY);

    Vector2D GetCenter() const { return m_Center; }
    float GetRotation() const { return m_Angle; }
    float GetScaleX() const { return m_Scale.x; }
    float GetScaleY() const { return m_Scale.y; }

    void Draw(const Utils::Color& _color = Utils::Color_White) const;

    bool Overlap(const Rect2D& _other) const;
    bool Overlap(const Ellipse2D& _elipse) const;

protected:
    void ComputePoints();

private:
    Vector2D m_Center;
    float m_Width = 0.f;
    float m_Height = 0.f;
    float m_Angle = 0.f;
    Vector2D m_Scale = Vector2D(1.f, 1.f);

    Vector2D m_Points[4];
};

class Square2D : public Rect2D
{
public:
    Square2D() {};
    Square2D(Vector2D& _center, float _size);

    void Scale(float _scale);
    float GetScale() const;

private:
    void Scale(float _scaleX, float _scaleY) { return Rect2D::Scale(_scaleX, _scaleY); }
    float GetScaleX() const { return Rect2D::GetScaleX(); }
    float GetScaleY() const { return Rect2D::GetScaleY(); }
};

class Ellipse2D
{
public:
    Ellipse2D() {};
    Ellipse2D(const Vector2D& _center, float _radius);

    void Move(const Vector2D& _pos);
    void Rotate(float _degrees);
    void Scale(float _scaleX, float _scaleY);

    Vector2D GetCenter() const { return m_Center; }
    float GetRadius(const Vector2D& _direction) const;

    void Draw(const Utils::Color& _color = Utils::Color_White, int sharpness = 10) const;

    bool Overlap(const Rect2D& _rect) const;
    bool Overlap(const Ellipse2D& _other) const;

protected:
    float m_Radius = 0.f;

private:
    Vector2D m_Center;
    Vector2D m_Scale = Vector2D(1.f, 1.f);
    float m_Angle;
};

class Circle2D : public Ellipse2D
{
public:
    Circle2D() {};
    Circle2D(const Vector2D& _center, float _radius);

    void Scale(float _scale);

    float GetRadius() const;

private:
    void Scale(float _scaleX, float _scaleY) { Ellipse2D::Scale(_scaleX, _scaleY); }
};