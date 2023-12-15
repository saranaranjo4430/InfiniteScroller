//------------------------------------------------------------------------
#include "stdafx.h"

#include "..\app\app.h"
#include "..\app\main.h"

#include "Utils.h"
#include "GameSprite.h"
#include "ScrollingBackground.h"
//------------------------------------------------------------------------

void ScrollingBackground::Init(const std::vector<const char*>& _sequence, const ScrollDirection& _direction)
{
    //Init Sprites
    for (auto texture : _sequence)
    {
        CGameSprite* sprite = new CGameSprite(texture, 1, 1);
        sprite->SetCollision(CollisionType::NONE);
        m_Backgrounds.push_back(sprite);
    }

    CGameSprite* sprite = new CGameSprite(_sequence.front(), 1, 1);
    sprite->SetCollision(CollisionType::NONE);
    m_Backgrounds.push_back(sprite);
    
    m_Direction = _direction;
}

void ScrollingBackground::Update(float _deltaTime)
{
    if (m_Direction == SCROLL_HORIZONTAL)
    {
        float vpXMin = gameVp.GetX(0.f);
        float vpXMax = gameVp.GetX(1.f);
        float scaleX = gameVp.GetScaleX(scale.x);

        float spriteOffset = 0.f;

        for (auto sprite : m_Backgrounds)
        {
            sprite->Update(_deltaTime);

            Vector2D& sizeInPixel = sprite->GetTextureSize();
            float offsetX = sizeInPixel.x * 0.5f * scaleX;
            offsetX -= m_ScrollPos * sizeInPixel.x * scaleX;
            offsetX += spriteOffset;

            sprite->position.x = offsetX / (vpXMax - vpXMin);
            sprite->position.y = position.y;
            sprite->scale = scale;

            spriteOffset += sizeInPixel.x * scaleX;
        }        
    }
    else if (m_Direction == SCROLL_VERTICAL)
    {
        float vpYMin = gameVp.GetY(0.f);
        float vpYMax = gameVp.GetY(1.f);
        float scaleY = gameVp.GetScaleY(scale.y);

        float spriteOffset = 0.f;

        for (auto sprite : m_Backgrounds)
        {
            sprite->Update(_deltaTime);

            Vector2D& sizeInPixel = sprite->GetTextureSize();
            float offsetY = sizeInPixel.y * 0.5f * scaleY;
            offsetY -= m_ScrollPos * sizeInPixel.y * scaleY;
            offsetY += spriteOffset;

            sprite->position.x = position.x;
            sprite->position.y = offsetY / (vpYMax - vpYMin);
            sprite->scale = scale;

            spriteOffset += sizeInPixel.y * scaleY;
        }
    }

    m_ScrollPos += scrollSpeed * _deltaTime/1000.f;

    float maxOffset = (float)(m_Backgrounds.size() - 1);
    if (m_ScrollPos > maxOffset)
        m_ScrollPos -= maxOffset;
}

void ScrollingBackground::Render()
{
    for (auto sprite : m_Backgrounds)
    {
        sprite->Render();
    }
}

void ScrollingBackground::Shutdown()
{
    for (auto sprite : m_Backgrounds)
    {
        delete sprite;
    }
}

void ScrollingBackground::Reset()
{ 
    m_ScrollPos = 0.f; 
}