#pragma once

namespace Utils
{
    struct Color
    {
        Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
        float r;
        float g;
        float b;
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
}