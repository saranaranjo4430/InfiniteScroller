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

float Vector2D::Distance(const Vector2D& _other) const
{
    Vector2D v = *this - _other;
    v.y /= gameVp.GetRatio();
    return v.GetNorm();
}

void Vector2D::Normalize(float _len)
{
    float norm = GetNorm();
    x = (x / norm) * _len;
    y = (y / norm) * _len;
}

void Vector2D::Rotate(float _degrees)
{
    _degrees = (_degrees > 180.f) ? -180.f + (_degrees - 180.f) : (_degrees < -180.f) ? 180.f - (-180.f - _degrees) : _degrees;

    float radians = Utils::DegToRad(_degrees);

    //To preserve AspectRatio while rotating
    y /= gameVp.GetRatio();

    float newX = (x * cosf(radians)) - (y * sinf(radians));
    float newY = (x * sinf(radians)) + (y * cosf(radians));

    //To preserve AspectRatio while rotating
    newY *= gameVp.GetRatio();

    x = newX;
    y = newY;
}

float Vector2D::Dot(const Vector2D& _other) const
{
    float dot = x * _other.x + y * _other.y;
    dot = Utils::Clampf(dot, -1.f, 1.f);
    return dot;
}

float Vector2D::Angle(const Vector2D& _other) const
{
    float dot = Dot(_other);
    float angle = acosf(dot);
    return Utils::RadToDeg(angle);
}

void Vector2D::Draw(const Vector2D& _center, const Utils::Color& _color) const
{
    float centerX = gameVp.GetX(_center.x);
    float centerY = gameVp.GetY(_center.y);
    Utils::DrawLine(centerX, centerY, centerX + gameVp.GetWidth(x), centerY + gameVp.GetHeight(y), _color);
}

//***********************************************************************
// Rectangle 2D
//***********************************************************************

Rect2D::Rect2D(const Vector2D& _center, float _width, float _height)
{
    m_Center = _center;
    m_Width = _width;
    m_Height = _height;

    ComputePoints();
}

void Rect2D::Move(const Vector2D& _pos)
{
    if (m_Center != _pos)
    {
        m_Center.x = _pos.x;
        m_Center.y = _pos.y;

        ComputePoints();
    }
}

void Rect2D::Rotate(float _degrees)
{
    _degrees = (_degrees > 180.f) ? -180.f + (_degrees - 180.f) : (_degrees < -180.f) ? 180.f - (-180.f - _degrees) : _degrees;

    if (m_Angle != _degrees)
    {
        m_Angle = _degrees;

        ComputePoints();
    }
}

void Rect2D::Scale(float _scaleX, float _scaleY)
{
    if (m_Scale.x != _scaleX || m_Scale.y != _scaleY)
    {
        m_Scale.x = _scaleX;
        m_Scale.y = _scaleY;

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_POLYGON);
    glColor4f(_color.r, _color.g, _color.b, _color.a);
    glVertex2f(ax, ay);
    glVertex2f(bx, by);
    glVertex2f(cx, cy);
    glVertex2f(dx, dy);
    glEnd();
    glDisable(GL_BLEND);
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

bool Rect2D::Overlap(const Ellipse2D& _ellipse) const
{
    // Rotate circle's center point back
    Vector2D unrotatedCircle = _ellipse.GetCenter() - m_Center;
    unrotatedCircle.Rotate(-m_Angle);
    unrotatedCircle += m_Center;

    /*
    Ellipse2D debug = _ellipse;
    debug.Move(unrotatedCircle);
    debug.Rotate(m_Angle);
    debug.Draw(Utils::Color_White);
    */

    // Closest point in the rectangle to the center of circle rotated backwards(unrotated)
    Vector2D unrotatedClosest = unrotatedCircle;

    float rectWidth = m_Width * m_Scale.x;
    float rectHeight = m_Height * m_Scale.y;

    float rectReferenceX = m_Center.x - rectWidth * 0.5f;
    float rectReferenceY = m_Center.y - rectHeight * 0.5f;

    // Find the unrotated closest x point from center of unrotated circle
    if (unrotatedCircle.x < rectReferenceX)
    {
        unrotatedClosest.x = rectReferenceX;
    }
    else if (unrotatedCircle.x > rectReferenceX + rectWidth)
    {
        unrotatedClosest.x = rectReferenceX + rectWidth;
    }

    // Find the unrotated closest y point from center of unrotated circle
    if (unrotatedCircle.y < rectReferenceY)
    {
        unrotatedClosest.y = rectReferenceY;
    }
    else if (unrotatedCircle.y > rectReferenceY + rectHeight)
    {
        unrotatedClosest.y = rectReferenceY + rectHeight;
    }

    // Come back to rotated space to get the ellipse directional vector
    Vector2D closest = unrotatedClosest - GetCenter();
    closest.Rotate(m_Angle);
    closest += GetCenter();
    Vector2D ellipseToRect = closest - _ellipse.GetCenter();
    
    // Determine collision
    float distance = unrotatedCircle.Distance(unrotatedClosest);
    return (distance <= _ellipse.GetRadius(ellipseToRect));
}

void Rect2D::ComputePoints()
{
    Vector2D right = Vector2D(1, 0);
    Vector2D up = Vector2D(0, 1);

    float halfWidth = m_Width * 0.5f * m_Scale.x;
    float halfHeight = m_Height * 0.5f * m_Scale.y;

    right.Normalize(halfWidth);
    up.Normalize(halfHeight);

    right.Rotate(m_Angle);
    up.Rotate(m_Angle);

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

void Square2D::Scale(float _scale) 
{ 
    return Scale(_scale, _scale); 
}

float Square2D::GetScale() const 
{ 
    return GetScaleX(); 
}

//***********************************************************************
// Ellipse 2D
//***********************************************************************

Ellipse2D::Ellipse2D(const Vector2D& _center, float _radius)
{
    m_Center = _center;
    m_Radius = _radius;
}

void Ellipse2D::Move(const Vector2D& _pos)
{
    m_Center.x = _pos.x;
    m_Center.y = _pos.y;
}

void Ellipse2D::Rotate(float _degrees)
{
    m_Angle = _degrees;
}

void Ellipse2D::Scale(float _scaleX, float _scaleY)
{
    m_Scale.x = _scaleX;
    m_Scale.y = _scaleY;
}

float Ellipse2D::GetRadius(const Vector2D& _direction) const
{
    if (_direction.GetNorm() < Utils::Epsilon())
    {
        return min(m_Radius * m_Scale.x, m_Radius * m_Scale.y);
    }

    Vector2D dir = _direction;
    dir.Rotate(-m_Angle);
    dir.Normalize();
    dir.x *= m_Radius * m_Scale.x;
    dir.y *= m_Radius * m_Scale.y;
    return dir.GetNorm();
}

void Ellipse2D::Draw(const Utils::Color& _color, int sharpness) const
{
    if (sharpness <= 0)
        return;

    float centerX = gameVp.GetX(m_Center.x);
    float centerY = gameVp.GetY(m_Center.y);

    float twicePi = 2.0f * PI;

    float baseAngle = Utils::DegToRad(m_Angle);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(_color.r, _color.g, _color.b, _color.a); // Yellow

    float vx = centerX;
    float vy = centerY;

#if APP_USE_VIRTUAL_RES
    APP_VIRTUAL_TO_NATIVE_COORDS(vx, vy);
#endif

    glVertex2f(vx, vy);

    int nbEdges = sharpness * 3;
    for (int i = 0; i <= nbEdges; i++)
    {
        float angle = i * twicePi / nbEdges;

        float deltaX = cosf(angle) * m_Radius * m_Scale.x;
        float deltaY = sinf(angle) * m_Radius * m_Scale.y;

        float newX = (deltaX * cosf(baseAngle)) - (deltaY * sinf(baseAngle));
        float newY = (deltaX * sinf(baseAngle)) + (deltaY * cosf(baseAngle));
        
        vx = centerX + gameVp.GetWidth(newX);
        vy = centerY + gameVp.GetHeight(newY) * gameVp.GetRatio();

#if APP_USE_VIRTUAL_RES
        APP_VIRTUAL_TO_NATIVE_COORDS(vx, vy);
#endif

        glVertex2f(vx, vy);
    }

    glEnd();
    glDisable(GL_BLEND);
}

bool Ellipse2D::Overlap(const Rect2D& _rect) const
{
    return _rect.Overlap(*this);
}

bool Ellipse2D::Overlap(const Ellipse2D& _other) const
{
    float distance = m_Center.Distance(_other.GetCenter());
    Vector2D dir = m_Center - _other.GetCenter();
    return (distance <= (GetRadius(-dir) + _other.GetRadius(dir)));
}

//***********************************************************************
// Circle 2D
//***********************************************************************

Circle2D::Circle2D(const Vector2D& _center, float _radius) : Ellipse2D(_center, _radius)
{

}

void Circle2D::Scale(float _scale) 
{ 
    Scale(_scale, _scale);
}

float Circle2D::GetRadius() const
{
    return m_Radius;
}