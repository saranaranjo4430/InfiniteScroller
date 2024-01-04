#pragma once

#include "..\MyGame\Utils\GameViewport.h"
#include "..\MyGame\Utils\Vectors.h"

#include "SpawnPoint.h"

class CGameSprite;
class CController;
class ScrollingBackground;
class MainCharacter;

class PaperGirl : public SpawnPointOwner
{
public:
    PaperGirl();
    ~PaperGirl();

    void Init(int width, int height);
    void Update(float _deltaTime);
    void Render();
    void Shutdown();

    void StartGame();
    void ResetGame();
    bool IsGameStarted() const { return m_GameStarted; }

    float GetBoostLevel() const { return m_BoostLevel; }

    const Range& GetScrollingRatio() const;
    float GetScrollingSpeed() const;

    bool RequestSpawnPointActivation(SpawnPoint* _point, float _posX);
    bool RequestSpawnPointRemoval(SpawnPoint* _point);

    float GetXInput() const;
    float GetYInput() const;

protected:
    void InitTitle();
    void UpdateTitle(float _deltaTime);
    void RenderTitle();

    void InitBackground();
    void UpdateBackground(float _deltaTime);
    void RenderBackground();

    void InitMainCharacter();

    void UpdateProps(float _deltaTime);
    void RenderProps();

    void UpdateLogic(float _deltaTime);

    SpawnPoint* CreateSpawnPoint(const SpawnPointType& _type, float _refPosY);
    void FreeSpawnPoint(SpawnPoint* _point);
    float GetClosestSpawnPoint(const SpawnPointType& _type, float _refPosX) const;

#ifdef _DEBUG
    void DrawDebug();
    void DrawDebugText(const char* _text, float _value);
#endif

private:
    const CController* m_ActiveController;

    MainCharacter* m_MainCharacter;
    ScrollingBackground* m_Background;

    std::vector<SpawnPoint*> m_SpawnPoints;
    std::vector<SpawnPoint*> m_SpawnPointsPool;
    std::vector<SpawnPoint*>::iterator m_SpawnPointsIt;

    CGameSprite* m_Title;
    CGameSprite* m_PressStart;

    bool m_GameStarted = false;
    bool m_PressStartVisible = true;
    float m_PressStartTimer = 0.f;

    float m_BoostLevel = 1.f;
    int m_NbCoins = 0;
    int m_NbNewspaper = 0;
};