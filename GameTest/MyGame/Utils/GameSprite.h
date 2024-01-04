#pragma once

#include "Vectors.h"
#include "GameViewport.h"

class CSimpleSprite;

//A GameSprite encapsulates a SimpleSprite. It is meant to display in a GameViewport.
//Position & Scale must be set in Viewport percents.

enum CollisionType
{
    NONE = 0,
    BOX,
    CIRCLE,
};

enum GameSpriteFlags
{
    DRAW_COLLISION = (1 << 0),
};

class CGameSprite
{
public:
    CGameSprite(const char* _fileName, int _columns, int _rows);
    virtual ~CGameSprite();

    void Update(float _deltaTime);
    void Render();

    inline void AddFlag(GameSpriteFlags flags) { m_Flags |= flags; }
    inline void RemoveFlag(GameSpriteFlags flags) { m_Flags &= ~flags; }
    inline bool HasFlag(GameSpriteFlags flags) { return (m_Flags & flags); }

    //Collisions
    void SetCollision(const CollisionType& type, const Vector2D& scale = Vector2D(1.f, 1.f), const Vector2D& pos = Vector2D(0.f, 0.f));

    bool Overlap(const CGameSprite* _other) const;
    bool Overlap(const Rect2D& _other) const;
    bool Overlap(const Ellipse2D& _other) const;

    //Animation Overrides
    void SetAnimation(int id, bool loop = true);
    void CreateAnimation(unsigned int id, float duration, const std::vector<int>& frames);
    bool IsFinished(int id) const;
    bool IsPlaying(int id) const;

    //Sprite Texture Overrides
    Vector2D GetPixelSize() const;
    Vector2D GetScreenSize() const;

public:
    Vector2D position = Vector2D(0.5f, 0.5f);
    Vector2D scale = Vector2D(1.f, 1.f);
    float angle = 0.f;

private:
    CSimpleSprite* m_BaseSprite = nullptr;

    Rect2D* m_CollisionBox = nullptr;
    Ellipse2D* m_CollisionCircle = nullptr;
    Vector2D m_CollisionPos = Vector2D(0.f, 0.f);
    Vector2D m_CollisionScale = Vector2D(1.f, 1.f);

    int m_Flags = 0;
};
