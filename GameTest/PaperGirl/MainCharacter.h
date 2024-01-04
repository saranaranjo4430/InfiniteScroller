#pragma once

#include "..\MyGame\Utils\GameViewport.h"
#include "..\MyGame\Utils\Vectors.h"

#include "Props.h"

class MainCharacter : public PropLogic
{
public:
    MainCharacter();
    ~MainCharacter();

    void Init();
    void Update(float _deltaTime);
    void Render();
    void Shutdown();

    void Reset();

    Vector2D GetPivotPos() const;

private:
    TheGirl* m_Prop = nullptr;
    TheGirl::Animation m_NextAnimation;
};