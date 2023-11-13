#pragma once

#include <map>
#include <vector>

#include "Vectors.h"
#include "GameViewport.h"

class CSimpleSprite;

//A GameSprite encapsulates a SimpleSprite. It is meant to display in a GameViewport.
//Position & Scale must be set in Viewport percents.

class CGameSprite
{
public:
    CGameSprite(const GameViewport& _vp, const char* _fileName, int _columns, int _rows);
    ~CGameSprite();

    void Update(float _deltaTime);
    void Render();

    //Overrides
    void SetAnimation(int id);
    void CreateAnimation(unsigned int id, float speed, const std::vector<int>& frames);

public:
    Vector2D position = Vector2D(0.f, 0.f);
    Vector2D scale = Vector2D(0.f, 0.f);
    float angle = 0.f;

private:
    const GameViewport& m_Vp;
    CSimpleSprite* m_BaseSprite;
};
