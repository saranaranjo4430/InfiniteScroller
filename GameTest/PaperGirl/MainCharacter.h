#pragma once

#include "..\MyGame\Utils\GameViewport.h"
#include "..\MyGame\Utils\Vectors.h"

#include "Props.h"

class MainCharacter : public PropManipulator
{
public:
    MainCharacter();
    ~MainCharacter();

    void Init();
    void Update(float _deltaTime);
    void Render();
    void Shutdown();

    void Reset();

    bool IsBoosting() const;

    Vector2D GetCenterPos() const;
    Vector2D GetPivotPos() const;

private:
    TheGirl* m_Prop = nullptr;
    TheGirl::Animation m_NextAnimation;
};