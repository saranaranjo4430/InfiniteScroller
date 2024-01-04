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
        m_Sequence.push_back(sprite);
    }

    CGameSprite* sprite = new CGameSprite(_sequence.front(), 1, 1);
    sprite->SetCollision(CollisionType::NONE);
    m_Sequence.push_back(sprite);
    
    m_Direction = _direction;
}

void ScrollingBackground::Update(float _deltaTime)
{
    m_ScrollPos += scrollSpeed * _deltaTime / 1000.f;

    bool res = false;
    while(!res)
    {
        UpdateSprites(_deltaTime);
        res = UpdateScrollingRatio(_deltaTime);
    }
}

void ScrollingBackground::Render()
{
    for (auto sprite : m_Sequence)
    {
        sprite->Render();
    }
}

void ScrollingBackground::Shutdown()
{
    for (auto sprite : m_Sequence)
    {
        delete sprite;
    }
}

void ScrollingBackground::Reset()
{ 
    m_ScrollPos = 0.f;
    m_VisibleSprite = nullptr;
    m_SequenceNbPixels = 0.f;
}

void ScrollingBackground::UpdateSprites(float _deltaTime)
{
    m_VisibleSprite = nullptr;
    m_SequenceNbPixels = 0.f;

    if (m_Direction == SCROLL_HORIZONTAL)
    {
        float vpWidth = gameVp.GetX(1.f) - gameVp.GetX(0.f);
        float scaleX = gameVp.GetScaleX(scale.x);

        for (auto sprite : m_Sequence)
        {
            float widthInPixel = sprite->GetPixelSize().x * scaleX;

            float offsetX = m_SequenceNbPixels + widthInPixel * 0.5f;

            sprite->position.x = offsetX / vpWidth - m_ScrollPos;
            sprite->position.y = position.y;
            sprite->scale = scale;

            sprite->Update(_deltaTime);

            //Visible sprite ?
            float minX = sprite->position.x * vpWidth - widthInPixel * 0.5f;
            float maxX = sprite->position.x * vpWidth + widthInPixel * 0.5f;
            if (minX <= 0.f && maxX >= 0.f)
            {
                assert(!m_VisibleSprite);
                m_VisibleSprite = sprite;
            }

            m_SequenceNbPixels += widthInPixel;
        }
    }
    else if (m_Direction == SCROLL_VERTICAL)
    {
        float vpHeight = gameVp.GetY(1.f) - gameVp.GetY(0.f);
        float scaleY = gameVp.GetScaleY(scale.y);

        for (auto sprite : m_Sequence)
        {
            float heightInPixel = sprite->GetPixelSize().y * scaleY;

            float offsetY = m_SequenceNbPixels + heightInPixel * 0.5f;

            sprite->position.x = position.x;
            sprite->position.y = offsetY / vpHeight - m_ScrollPos;
            sprite->scale = scale;

            sprite->Update(_deltaTime);

            //Visible sprite ?
            float minY = sprite->position.y * vpHeight - heightInPixel * 0.5f;
            float maxY = sprite->position.y * vpHeight + heightInPixel * 0.5f;
            if (minY <= 0.f && maxY >= 0.f)
            {
                assert(!m_VisibleSprite);
                m_VisibleSprite = sprite;
            }

            m_SequenceNbPixels += heightInPixel;
        }
    }
}

bool ScrollingBackground::UpdateScrollingRatio(float _deltaTime)
{
    //We've probably gone too far, reset
    if (!m_VisibleSprite)
    {
        m_ScrollPos = 0.f;
        return false;
    }

    if (m_Direction == SCROLL_HORIZONTAL)
    {
        float vpWidth = gameVp.GetX(1.f) - gameVp.GetX(0.f);
        float scaleX = gameVp.GetScaleX(scale.x);

        float widthInPixel = m_VisibleSprite->GetPixelSize().x * scaleX;
        float minX = m_VisibleSprite->position.x * vpWidth - widthInPixel * 0.5f;

        //End of sequence, loop
        if (m_VisibleSprite == m_Sequence.back())
        {
            m_ScrollPos = -minX / widthInPixel;
            return false;
        }
        
        //Compute Ratio according to currently visible sprite
        m_ScrollingRatio.min = m_SequenceNbPixels - minX;
        m_ScrollingRatio.max = m_ScrollingRatio.min + vpWidth;

        m_ScrollingRatio.min /= m_SequenceNbPixels;
        m_ScrollingRatio.max /= m_SequenceNbPixels;
    }
    else if (m_Direction == SCROLL_VERTICAL)
    {
        float vpHeight = gameVp.GetY(1.f) - gameVp.GetY(0.f);
        float scaleY = gameVp.GetScaleY(scale.y);

        float heightInPixel = m_VisibleSprite->GetPixelSize().y * scaleY;
        float minY = m_VisibleSprite->position.y * vpHeight - heightInPixel * 0.5f;

        //End of sequence, loop
        if (m_VisibleSprite == m_Sequence.back())
        {
            m_ScrollPos = -minY / heightInPixel;
            return false;
        }
        
        m_ScrollingRatio.min = m_SequenceNbPixels - minY;
        m_ScrollingRatio.max = m_ScrollingRatio.min + vpHeight;

        m_ScrollingRatio.min /= m_SequenceNbPixels;
        m_ScrollingRatio.max /= m_SequenceNbPixels;
    }

    return true;
}

