//------------------------------------------------------------------------
#include "stdafx.h"

#include "..\app\app.h"
#include "..\app\main.h"

#include "Utils.h"
#include "GameViewport.h"
//------------------------------------------------------------------------

GameViewport gameVp;

void GameViewport::Init(int _width, int _height)
{
    m_VpWidth = _width;
    m_VpHeight = _height;

    m_VpRatio = (float)m_VpWidth / (float)m_VpHeight;
}

void GameViewport::Render()
{
    //Viewport corners
    float bottomLeftX = GetX(0.f);
    float bottomLeftY = GetY(0.f);
    float topRightX = GetX(1.f);
    float topRightY = GetY(1.f);

    if (HasFlag(DRAW_VIEWPORT))
    {
        //Viewport polygon
        Utils::DrawPolygon(bottomLeftX, bottomLeftY, topRightX, topRightY, Utils::Color_Blue);
    }
}

void GameViewport::RenderBorders()
{
    //Viewport corners
    float bottomLeftX = GetX(0.f);
    float bottomLeftY = GetY(0.f);
    float topRightX = GetX(1.f);
    float topRightY = GetY(1.f);

#if APP_USE_VIRTUAL_RES
    float min = 0.f;
    float xmax = APP_INIT_WINDOW_WIDTH;
    float ymax = APP_INIT_WINDOW_HEIGHT;
#else
    float min = -1.f;
    float xmax = 1.f;
    float ymax = 1.f;
#endif

    //Left & Bottom polygons
    Utils::DrawPolygon(min, bottomLeftY, bottomLeftX, topRightY, Utils::Color_Black);
    Utils::DrawPolygon(min, min, xmax, bottomLeftY, Utils::Color_Black);

    //Right & Top polygons
    Utils::DrawPolygon(topRightX, bottomLeftY, xmax, topRightY, Utils::Color_Black);
    Utils::DrawPolygon(min, topRightY, xmax, ymax, Utils::Color_Black);
}

float GameViewport::GetX(float _percent) const
{
    float x = GetXInPixel(_percent);

#if !APP_USE_VIRTUAL_RES
    x = (x / APP_INIT_WINDOW_WIDTH) * 2.f - 1.f;
#endif

    return x;
}

float GameViewport::GetY(float _percent) const
{
    float y = GetYInPixel(_percent);

#if !APP_USE_VIRTUAL_RES
    y = (y / APP_INIT_WINDOW_HEIGHT) * 2.f - 1.f;
#endif

    return y;
}

float GameViewport::GetWidth(float _percent) const
{
    float min = GetX(0.f);
    float max = GetX(1.f);
    float size = (max - min) * _percent;
    return size;
}

float GameViewport::GetHeight(float _percent) const
{
    float min = GetY(0.f);
    float max = GetY(1.f);
    float size = (max - min) * _percent;
    return size;
}

float GameViewport::GetScaleX(float _scale) const
{
    float ratio = GetWidth(1.f) / (float)m_VpHeight;
    ratio *= _scale;
    ratio /= m_VpRatio;
    return ratio;
}

float GameViewport::GetScaleY(float _scale) const
{
    float ratio = GetHeight(1.f) / (float)m_VpHeight;
    ratio *= _scale;
    return ratio;
}

float GameViewport::GetVirtualScale(float _scale) const
{
    float scale = GetScaleX(_scale);

    float appWindowRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    if (appWindowRatio > m_VpRatio)
    {
        scale *= appWindowRatio / m_VpRatio;
    }
    return scale;
}

float GameViewport::GetXInPixel(float _percent) const
{
    float appWindowRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    if (appWindowRatio > m_VpRatio)
    {
        //App Window is larger than the Game Viewport
        float vpSizeWithinAppWindow = (m_VpRatio / appWindowRatio) * APP_INIT_WINDOW_WIDTH;
        float vpOffset = (APP_INIT_WINDOW_WIDTH - vpSizeWithinAppWindow) * 0.5f;
        float x = vpOffset + (vpSizeWithinAppWindow * _percent);
        return x;
    }
     
    return APP_INIT_WINDOW_WIDTH * _percent;
}

float GameViewport::GetYInPixel(float _percent) const
{
    float appWindowRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    if (appWindowRatio < m_VpRatio)
    {
        //App Window is smaller than the Game Viewport
        float vpSizeWithinAppWindow = (appWindowRatio / m_VpRatio) * APP_INIT_WINDOW_HEIGHT;
        float vpOffset = (APP_INIT_WINDOW_HEIGHT - vpSizeWithinAppWindow) * 0.5f;
        float y = vpOffset + (vpSizeWithinAppWindow * _percent);
        return y;
    }

    return APP_INIT_WINDOW_HEIGHT * _percent;
}
