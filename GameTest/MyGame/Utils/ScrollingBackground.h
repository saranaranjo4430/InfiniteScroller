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

    void Reset();

public:
    Vector2D position = Vector2D(0.5f, 0.5f);
    Vector2D scale = Vector2D(1.f, 1.f);
    float scrollSpeed = 0.f;

private:
    std::vector<CGameSprite*> m_Backgrounds;
    ScrollDirection m_Direction;

    float m_ScrollPos = 0.f;
};

