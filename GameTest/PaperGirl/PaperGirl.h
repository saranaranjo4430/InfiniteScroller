#pragma once

#include "..\MyGame\Utils\GameViewport.h"
#include "..\MyGame\Utils\Vectors.h"

#include "SpawnPoint.h"

class CGameSprite;
class CController;
class ScrollingBackground;
class MainCharacter;

class Frequency
{
public:
    Frequency() {};

    void Init(const Range& _range, float* _rangeFactor, float _period);
    void Update(float _deltaTime);

    bool Hits();

private:
    Range m_Range;
    float* m_RangeFactor = nullptr;

    float m_Frequency = 0.f;
    float m_Period = 0.f;
    float m_Time = 0.f;
    bool m_Available = false;
};

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

    void InitProps();
    void UpdateProps(float _deltaTime);
    void RenderProps();

    void InitLogic();
    void UpdateLogic(float _deltaTime);

    SpawnPoint* CreateSpawnPoint(const SpawnPointType& _type, float _refPosY);
    void FreeSpawnPoint(SpawnPoint* _point);
    float GetClosestSpawnPoint(const SpawnPointType& _type, float _refPosX) const;

#ifdef _DEBUG
    void DrawDebug();
    void DrawDebugText(const char* _text, int _value);
    void DrawDebugText(const char* _text, float _value);
#endif

private:
    const CController* m_ActiveController;

    MainCharacter* m_MainCharacter;
    
    std::vector<SpawnPoint*> m_SpawnPoints;
    std::vector<SpawnPoint*> m_SpawnPointsPool;
    std::vector<SpawnPoint*>::iterator m_SpawnPointsIt;

    ScrollingBackground* m_Background;

    CGameSprite* m_Title;
    CGameSprite* m_PressStart;

    bool m_GameStarted = false;
    bool m_PressStartVisible = true;
    float m_PressStartTimer = 0.f;

    float m_DifficultyFactor = 0.f;                   //0.f easy, 1.f hell on earth
    float m_BoostLevel = 1.f;
    float m_NbCoins = 0.f;
    int m_NbNewspaper = 0;

    Frequency m_RecycleBinfreq;
    Frequency m_MailBoxfreq;
    Frequency m_GrandMafreq;
};