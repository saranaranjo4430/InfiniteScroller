#pragma once

#include <map>
#include <vector>

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
    void SetCollision(const CollisionType& type, const Vector2D& scale = Vector2D(1.f, 1.f));

    bool Overlap(const CGameSprite* _other) const;
    bool Overlap(const Rect2D& _other) const;
    bool Overlap(const Ellipse2D& _other) const;

    //Overrides
    void SetAnimation(int id);
    void CreateAnimation(unsigned int id, float speed, const std::vector<int>& frames);

public:
    Vector2D position = Vector2D(0.5f, 0.5f);
    Vector2D scale = Vector2D(1.f, 1.f);
    float angle = 0.f;

private:
    CSimpleSprite* m_BaseSprite = nullptr;

    Rect2D* m_CollisionBox = nullptr;
    Ellipse2D* m_CollisionCircle = nullptr;
    Vector2D m_CollisionScale = Vector2D(1.f, 1.f);

    int m_Flags = 0;
};
