//------------------------------------------------------------------------
#include "stdafx.h"
#include <math.h>

#include "..\app\app.h"

#include "Vectors.h"
//------------------------------------------------------------------------

//***********************************************************************
// Vector 2D
//***********************************************************************

float Vector2D::GetNorm() const 
{
    return sqrtf(GetNorm2());
}

float Vector2D::GetNorm2() const 
{
    return x * x + y * y;
}

void Vector2D::Normalize(float _len)
{
    float norm = GetNorm();
    x = (x / norm) * _len;
    y = (y / norm) * _len;
}

void Vector2D::Rotate(float _degrees)
{
    _degrees = (_degrees > 180.f) ? (_degrees - 180.f) : (_degrees < -180.f) ? (180.f + _degrees) : _degrees;

    float radians = Utils::DegToRad(_degrees);

    float newX = (x * cosf(radians)) - (y * sinf(radians));
    float newY = (x * sinf(radians)) + (y * cosf(radians));

    x = newX;
    y = newY;
}

float Vector2D::Dot(const Vector2D& _other)
{
    float dot = x * _other.x + y * _other.y;
    dot = Utils::Clampf(dot, -1.f, 1.f);
    return dot;
}

float Vector2D::Angle(const Vector2D& _other)
{
    float dot = Dot(_other);
    float angle = acosf(dot);
    return Utils::RadToDeg(angle);
}

//***********************************************************************
// Rectangle 2D
//***********************************************************************

Rect2D::Rect2D(Vector2D& _center, float _width, float _height)
{
    m_Center = _center;
    m_Width = _width;
    m_Height = _height;
    
    ComputePoints();
}

void Rect2D::Move(float _x, float _y)
{
    if (m_Center.x != _x || m_Center.y != _y)
    {
        m_Center.x = _x;
        m_Center.y = _y;

        ComputePoints();
    }
}

void Rect2D::Rotate(float _degrees)
{
    _degrees = (_degrees > 180.f) ? (_degrees - 180.f) : (_degrees < -180.f) ? (180.f + _degrees) : _degrees;

    if (m_Angle != _degrees)
    {
        m_Angle = _degrees;

        ComputePoints();
    }
}

void Rect2D::Scale(float _scale)
{
    if (m_Scale != _scale)
    {
        m_Scale = _scale;

        ComputePoints();
    }
}

void Rect2D::Draw(const Utils::Color& _color) const
{
    //Convert from viewport to pixels
    float ax = gameVp.GetX(m_Points[0].x);
    float ay = gameVp.GetY(m_Points[0].y);

    float bx = gameVp.GetX(m_Points[1].x);
    float by = gameVp.GetY(m_Points[1].y);

    float cx = gameVp.GetX(m_Points[2].x);
    float cy = gameVp.GetY(m_Points[2].y);

    float dx = gameVp.GetX(m_Points[3].x);
    float dy = gameVp.GetY(m_Points[3].y);
        
    //Draw
#if APP_USE_VIRTUAL_RES
    APP_VIRTUAL_TO_NATIVE_COORDS(ax, ay);
    APP_VIRTUAL_TO_NATIVE_COORDS(bx, by);
    APP_VIRTUAL_TO_NATIVE_COORDS(cx, cy);
    APP_VIRTUAL_TO_NATIVE_COORDS(dx, dy);
#endif

    glBegin(GL_POLYGON);
    glColor3f(_color.r, _color.g, _color.b);
    glVertex2f(ax, ay);
    glVertex2f(bx, by);
    glVertex2f(cx, cy);
    glVertex2f(dx, dy);
    glEnd();
}

bool Rect2D::Overlap(const Rect2D& _other) const
{
    for (int id = 0; id < 2; id++)
    {
        const Rect2D& rect = (id == 0) ? *this : _other;

        for (int i1 = 0; i1 < 4; i1++)
        {
            int i2 = (i1 + 1) % 4;

            const Vector2D& p1 = rect.m_Points[i1];
            const Vector2D& p2 = rect.m_Points[i2];

            Vector2D normal = Vector2D(p2.y - p1.y, p1.x - p2.x);

            float minA = 10000.f;
            float maxA = -10000.f;

            for (const Vector2D& p : m_Points)
            {
                float projected = normal.x * p.x + normal.y * p.y;

                if (projected < minA) minA = projected;
                if (projected > maxA) maxA = projected;
            }

            float minB = 10000.f;
            float maxB = -10000.f;

            for (const Vector2D& p : _other.m_Points)
            {
                float projected = normal.x * p.x + normal.y * p.y;

                if (projected < minB) minB = projected;
                if (projected > maxB) maxB = projected;
            }

            if (maxA < minB || maxB < minA)
            {
                return false;
            }
        }
    }

    return true;
}

bool Rect2D::Overlap(const Circle2D& _circle) const
{
    float radians = Utils::DegToRad(m_Angle);

    float circleX = _circle.GetCenter().x;
    float circleY = _circle.GetCenter().y;

    // Rotate circle's center point back
    float unrotatedCircleX = cosf(-radians) * (circleX - m_Center.x) - sinf(-radians) * (circleY - m_Center.y) + m_Center.x;
    float unrotatedCircleY = sinf(-radians) * (circleX - m_Center.x) + cosf(-radians) * (circleY - m_Center.y) + m_Center.y;

    // Closest point in the rectangle to the center of circle rotated backwards(unrotated)
    float closestX = unrotatedCircleX;
    float closestY = unrotatedCircleY;

    float rectWidth = m_Width * m_Scale;
    float rectHeight = m_Height * m_Scale;

    float rectReferenceX = m_Center.x - rectWidth * 0.5f;
    float rectReferenceY = m_Center.y - rectHeight * 0.5f;

    // Find the unrotated closest x point from center of unrotated circle
    if (unrotatedCircleX < rectReferenceX) 
    {
        closestX = rectReferenceX;
    }
    else if (unrotatedCircleX > rectReferenceX + rectWidth)
    {
        closestX = rectReferenceX + rectWidth;
    }

    // Find the unrotated closest y point from center of unrotated circle
    if (unrotatedCircleY < rectReferenceY) 
    {
        closestY = rectReferenceY;
    }
    else if (unrotatedCircleY > rectReferenceY + rectHeight)
    {
        closestY = rectReferenceY + rectHeight;
    }

    // Determine collision
    float dX = abs(unrotatedCircleX - closestX);
    float dY = abs(unrotatedCircleY - closestY) / gameVp.GetRatio();
    float distance = sqrtf((dX * dX) + (dY * dY));

    return (distance < _circle.GetRadius());
}

void Rect2D::ComputePoints()
{
    Vector2D right = Vector2D(1, 0);
    Vector2D up = Vector2D(0, 1);

    right.Rotate(m_Angle);
    up.Rotate(m_Angle);

    float halfWidth = m_Width * 0.5f * m_Scale;
    float halfHeight = m_Height * 0.5f * m_Scale / gameVp.GetRatio();

    right.Normalize(halfWidth);
    up.Normalize(halfHeight);
    
    right.y *= gameVp.GetRatio();
    up.y *= gameVp.GetRatio();

    m_Points[0] = m_Center - right + up;
    m_Points[1] = m_Center + right + up;
    m_Points[2] = m_Center + right - up;
    m_Points[3] = m_Center - right - up;
}

//***********************************************************************
// Square 2D
//***********************************************************************

Square2D::Square2D(Vector2D& _center, float _size) : Rect2D(_center, _size, _size* gameVp.GetRatio())
{
}

//***********************************************************************
// Circle 2D
//***********************************************************************

Circle2D::Circle2D(Vector2D& _center, float _radius)
{
    m_Center = _center;
    m_Radius = _radius;
}

void Circle2D::Move(float _x, float _y)
{
    if (m_Center.x != _x || m_Center.y != _y)
    {
        m_Center.x = _x;
        m_Center.y = _y;
    }
}

void Circle2D::Draw(const Utils::Color& _color, int sharpness) const
{
    if (sharpness <= 0)
        return;

    float centerX = gameVp.GetX(m_Center.x);
    float centerY = gameVp.GetY(m_Center.y);

    float twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(_color.r, _color.g, _color.b); // Yellow

    float vx = centerX;
    float vy = centerY;

#if APP_USE_VIRTUAL_RES
    APP_VIRTUAL_TO_NATIVE_COORDS(vx, vy);
#endif

    glVertex2f(vx, vy);

    int nbEdges = sharpness * 3;
    for (int i = 0; i <= nbEdges; i++) 
    {
        float deltaX = m_Radius * cosf(i * twicePi / nbEdges);
        float deltaY = m_Radius * sinf(i * twicePi / nbEdges);

        vx = centerX + gameVp.GetWidth(deltaX);
        vy = centerY + gameVp.GetHeight(deltaY) * gameVp.GetRatio();

#if APP_USE_VIRTUAL_RES
        APP_VIRTUAL_TO_NATIVE_COORDS(vx, vy);
#endif

        glVertex2f(vx, vy);
    }

    glEnd();
}

bool Circle2D::Overlap(const Rect2D& _rect) const
{
    return _rect.Overlap(*this);
}

bool Circle2D::Overlap(const Circle2D& _other) const
{
    float distance = (m_Center - _other.GetCenter()).GetNorm();
    return (distance < (m_Radius + _other.GetRadius()));
}

