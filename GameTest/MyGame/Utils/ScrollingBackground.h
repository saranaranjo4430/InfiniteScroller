#pragma once

class CGameSprite;

enum ScrollDirection
{
    SCROLL_HORIZONTAL = 0,
    SCROLL_VERTICAL
};

class ScrollingBackground
{
public:
    ScrollingBackground() {};

    void Init(const std::vector<const char*>& _sequence, const ScrollDirection& _direction);
    void Update(float _deltaTime);
    void Render();
    void Shutdown();

    const Range& GetScrollingRatio() const { return m_ScrollingRatio; }

    void Reset();

public:
    Vector2D position = Vector2D(0.5f, 0.5f);
    Vector2D scale = Vector2D(1.f, 1.f);
    float scrollSpeed = 0.f;

protected:
    void UpdateSprites(float _deltaTime);
    bool UpdateScrollingRatio(float _deltaTime);

private:
    std::vector<CGameSprite*> m_Sequence;
    ScrollDirection m_Direction;

    float m_ScrollPos = 0.f;
    Range m_ScrollingRatio;

    CGameSprite* m_VisibleSprite = nullptr;
    float m_SequenceNbPixels = 0.f;
};

