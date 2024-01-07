#pragma once

#define safedelete(p) if (p) delete p; p=nullptr;
#define STICK_THRESHOLD 0.2f

class CController;

namespace Utils
{
    struct Color
    {
        Color(float _r, float _g, float _b, float _a = 1.f) : r(_r), g(_g), b(_b), a(_a) {}
        float r;
        float g;
        float b;
        float a;
    };

    static Color Color_Red = Color(1, 0, 0);
    static Color Color_Green = Color(0, 1, 0);
    static Color Color_Blue = Color(0, 0, 1);
    static Color Color_Yellow = Color(1, 1, 0);
    static Color Color_White = Color(1, 1, 1);
    static Color Color_Black = Color(0, 0, 0);
    static Color Color_Grey = Color(0.5f, 0.5f, 0.5f);

    void DrawPolygon(float _sx, float _sy, float _ex, float _ey, const Color& _col);
    void DrawLine(float _sx, float _sy, float _ex, float _ey, const Color& _col);
    
    const CController* GetFirstActiveController();

    float Clampf(float value, float min, float max);
    float DegToRad(float degrees);
    float RadToDeg(float radians);
    float Random(float fMin, float fMax);
    float Epsilon();
}

struct Range
{
    Range() { }
    Range(float _min, float _max)
    {
        min = _min;
        max = _max;
    };

    float min = 0.f;
    float max = 0.f;
};