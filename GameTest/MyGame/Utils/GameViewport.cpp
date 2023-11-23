//------------------------------------------------------------------------
#include "stdafx.h"

#include "..\app\app.h"
#include "..\app\main.h"

#include "Utils.h"
#include "GameViewport.h"

//------------------------------------------------------------------------

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
        Utils::DrawPolygon(bottomLeftX, bottomLeftY, topRightX, topRightY, 0, 0, 1);

        // Bind the background texture
        glBindTexture(GL_TEXTURE_2D, bgTextureID);

        // Render a quad with the background texture
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(bottomLeftX, bottomLeftY);  // Bottom Left
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(topRightX, bottomLeftY);    // Bottom Right
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(topRightX, topRightY);      // Top Right
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(bottomLeftX, topRightY);    // Top Left
        glEnd();

       
    }
    
    if (HasFlag(DRAW_BORDERS))
    {
        //Left & Bottom polygons
        Utils::DrawPolygon(0, bottomLeftY, bottomLeftX, topRightY, 0, 1, 0);
        Utils::DrawPolygon(0, 0, APP_INIT_WINDOW_WIDTH, bottomLeftY, 1, 0, 0);

        //Right & Top polygons
        Utils::DrawPolygon(topRightX, bottomLeftY, APP_INIT_WINDOW_WIDTH, topRightY, 0, 1, 0);
        Utils::DrawPolygon(0, topRightY, APP_INIT_WINDOW_WIDTH, APP_INIT_WINDOW_HEIGHT, 1, 0, 0);
    }
}

float GameViewport::GetX(float _percent) const
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


float GameViewport::GetY(float _percent) const
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



