#pragma once

#include "..\MyGame\Utils\GameViewport.h"
#include "..\MyGame\Utils\Vectors.h"

class CGameSprite;
class CController;
class ScrollingBackground;

class PaperGirl
{
public:
    PaperGirl();
    ~PaperGirl();

    void Init(int width, int height);
    void Update(float _deltaTime);
    void Render();
    void Shutdown();

protected:
    void InitBackground();

    void InitMainCharacter();
    void UpdateMainCharacter(float _deltaTime);
    void UpdateMainCharacterAnim();

    void StartRide();
    void ResetGame();

    float GetBoostLevel() const;

    float GetXInput() const;
    float GetYInput() const;

private:
    const CController* m_ActiveController;

    enum MainCharacterAnim
    {
        ANIM_IDLE = 0,
        
        ANIM_MOVE_SLOW,
        ANIM_MOVE_NORMAL,
        ANIM_MOVE_FAST,
        
        ANIM_TRANSITION_START,
        ANIM_TRANSITION_BOOST,

        ANIM_ACTION_THROW,
        ANIM_ACTION_FALL
    };
    MainCharacterAnim m_MainCharacterWantedAnimState;
    CGameSprite* m_MainCharacter;

    ScrollingBackground* m_Background;

    bool m_RideStarted = false;
    float m_BoostLevel = 1.f;
};