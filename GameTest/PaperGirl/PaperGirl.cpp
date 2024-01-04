//------------------------------------------------------------------------
#include <stdafx.h>
#include <algorithm>

#include "..\app\app.h"

#include "..\MyGame\Utils\Utils.h"
#include "..\MyGame\Utils\GameSprite.h"
#include "..\MyGame\Utils\ScrollingBackground.h"

#include "Constants.h"
#include "PaperGirl.h"
#include "MainCharacter.h"
//------------------------------------------------------------------------

PaperGirl::PaperGirl()
{
}

PaperGirl::~PaperGirl()
{
}

void PaperGirl::Init(int width, int height)
{
    gameVp.Init(width, height);
    gameVp.AddFlag(ViewportFlags::DRAW_VIEWPORT);
    //m_GameVp.AddFlag(ViewportFlags::DRAW_BORDERS);

    InitTitle();
    InitBackground();
    InitMainCharacter();

    ResetGame();
}

void PaperGirl::Update(float _deltaTime)
{
    m_ActiveController = Utils::GetFirstActiveController();

    gameVp.Update(_deltaTime);

    UpdateLogic(_deltaTime);
    UpdateTitle(_deltaTime);
    UpdateBackground(_deltaTime);
    UpdateProps(_deltaTime);
}

void PaperGirl::Render()
{
    gameVp.Render();
    {
        RenderBackground();
        RenderProps();
        RenderTitle();
    }
    gameVp.RenderBorders();

#ifdef _DEBUG
    DrawDebug();
#endif
}

void PaperGirl::Shutdown()
{
    m_MainCharacter->Shutdown();
    delete m_MainCharacter;

    m_Background->Shutdown();
    delete m_Background;

    delete m_Title;
    delete m_PressStart;

    for(auto point : m_SpawnPoints)
    {
        point->Shutdown();
        delete point;
    }
    m_SpawnPoints.clear();

    for (auto point : m_SpawnPointsPool)
    {
        point->Shutdown();
        delete point;
    }
    m_SpawnPointsPool.clear();

    gameVp.Shutdown();
}

void PaperGirl::InitTitle()
{
    m_Title = new CGameSprite(".\\PaperGirl\\Data\\MainTitle.bmp", 1, 1);
    m_PressStart = new CGameSprite(".\\PaperGirl\\Data\\PressStart.bmp", 1, 1);

    m_Title->position.y = 0.75f;
    m_PressStart->position.y = 0.575f;
    m_PressStart->scale = Vector2D(0.45f, 0.45f);
}

void PaperGirl::UpdateTitle(float _deltaTime)
{
    if (!m_GameStarted)
    {
        m_PressStartTimer += _deltaTime / 1000.f;
        if (m_PressStartTimer >= Constants::Interface::pressStartPulseDuration)
        {
            m_PressStartTimer = 0.f;
            m_PressStartVisible = !m_PressStartVisible;
        }

        m_Title->Update(_deltaTime);
        m_PressStart->Update(_deltaTime);
    }
}

void PaperGirl::RenderTitle()
{
    if (!m_GameStarted)
    {
        m_Title->Render();

        (m_PressStartVisible) ? m_PressStart->Render() : 0;
    }
}

void PaperGirl::InitBackground()
{
    std::vector<const char*> bitmaps;
    bitmaps.push_back(".\\PaperGirl\\Data\\Background01.bmp");

    //Init Scrolling
    m_Background = new ScrollingBackground();
    m_Background->Init(bitmaps, SCROLL_HORIZONTAL);
    m_Background->scale = Vector2D(Constants::Background::scale, Constants::Background::scale);
    m_Background->position.y = 0.5f * Constants::Background::scale;
    m_Background->scrollSpeed = 0.0f;
        
    //Init GrandMa predefined positions
    float minRatio = 0.1f;
    float maxRatio = (bitmaps.size() + 1) - 0.1f;

    float nbGrandMa = 20;
    for (int i = 0; i < nbGrandMa; i++)
    {
        float ratio = minRatio + (maxRatio - minRatio) * (i / (nbGrandMa - 1));

        SpawnPoint* spawnPoint = CreateSpawnPoint(SpawnPointType::GRANDMA, 0.35f);
        spawnPoint->AttachToBackground(ratio);
    }
}

void PaperGirl::UpdateBackground(float _deltaTime)
{
    m_Background->Update(_deltaTime);

    if (m_GameStarted)
    {
        //Scrolling speed
        float target = Constants::Gameplay::scrollSpeed.min + (Constants::Gameplay::scrollSpeed.max - Constants::Gameplay::scrollSpeed.min) * Constants::Gameplay::difficultyFactor;
        if (m_Background->scrollSpeed < target)
        {
            m_Background->scrollSpeed += Constants::Gameplay::scrollAccSpeed;
            (m_Background->scrollSpeed > target) ? m_Background->scrollSpeed = target : 0;
        }
        else
        {
            m_Background->scrollSpeed -= Constants::Gameplay::scrollAccSpeed;
            (m_Background->scrollSpeed < target) ? m_Background->scrollSpeed = target : 0;
        }
    }
}

void PaperGirl::RenderBackground()
{
    m_Background->Render();
}

void PaperGirl::InitMainCharacter()
{
    m_MainCharacter = new MainCharacter();
    m_MainCharacter->Init();
}

void PaperGirl::UpdateProps(float _deltaTime)
{
    m_MainCharacter->Update(_deltaTime);

    for(m_SpawnPointsIt = m_SpawnPoints.begin(); m_SpawnPointsIt < m_SpawnPoints.end();)
    {
        SpawnPoint* point = *m_SpawnPointsIt;
        m_SpawnPointsIt++;

        point->Update(_deltaTime);
    }
}

void PaperGirl::RenderProps()
{
    bool mainCharacterRendered = false;
    
    for (auto point : m_SpawnPoints)
    {
        if (!point->IsSpawned())
            continue;

        //Insert PaperGirl
        if (!mainCharacterRendered && m_MainCharacter->GetPivotPos().y > point->GetPivotPos().y)
        {
            m_MainCharacter->Render();
            mainCharacterRendered = true;
        }

        point->Render();
    }

    if (!mainCharacterRendered)
    {
        m_MainCharacter->Render();
    }
}

void PaperGirl::UpdateLogic(float _deltaTime)
{
    if (m_GameStarted)
    {
        if (m_ActiveController && m_ActiveController->CheckButton(XINPUT_GAMEPAD_BACK, false))
        {
            ResetGame();
        }

        //Increase difficulty depending on time
        {
            Constants::Gameplay::difficultyFactor += Constants::Gameplay::difficultyAccSpeed * _deltaTime / 1000.f;
            (Constants::Gameplay::difficultyFactor > 1.f) ? Constants::Gameplay::difficultyFactor = 1.f : 0;
        }

        //Spawn Obstacles
        {
            //Don't spawn if a MailBox is too close
            float minDistFromOther = GetClosestSpawnPoint(SpawnPointType::MAILBOX, Constants::Gameplay::offscreenSafePosX.max);
            if (minDistFromOther >= 0.1f)
            {
                float frequency = Constants::Gameplay::recycleBinFrequency.min + (Constants::Gameplay::recycleBinFrequency.max - Constants::Gameplay::recycleBinFrequency.min) * Constants::Gameplay::difficultyFactor;
                if (Utils::Random(0, 1) < frequency)
                {
                    //Get Closest recycleBin
                    float minDist = GetClosestSpawnPoint(SpawnPointType::RECYCLEBIN, Constants::Gameplay::offscreenSafePosX.max);
                    float minAllowedSpace = Constants::Gameplay::recycleBinSpacing.min + (Constants::Gameplay::recycleBinSpacing.max - Constants::Gameplay::recycleBinSpacing.min) * Constants::Gameplay::difficultyFactor;

                    if (minDist >= minAllowedSpace)
                    {
                        float posY = Constants::Gameplay::roadHeight.min + (Constants::Gameplay::roadHeight.max - Constants::Gameplay::roadHeight.min) * Utils::Random(0, 1);
                        posY -= 0.02f; //Small delta to compensate texture size

                        CreateSpawnPoint(SpawnPointType::RECYCLEBIN, posY);
                    }
                }
            }
        }

        //Spawn Mailboxes
        {
            //Don't spawn if an obsctacle is too close
            float minDistFromOther = GetClosestSpawnPoint(SpawnPointType::RECYCLEBIN, Constants::Gameplay::offscreenSafePosX.max);
            if (minDistFromOther >= 0.1f)
            {
                float frequency = Constants::Gameplay::mailBoxFrequency.min + (Constants::Gameplay::mailBoxFrequency.max - Constants::Gameplay::mailBoxFrequency.min) * Constants::Gameplay::difficultyFactor;
                if (Utils::Random(0, 1) < frequency)
                {
                    //Get Closest recycleBin
                    float minDist = GetClosestSpawnPoint(SpawnPointType::MAILBOX, Constants::Gameplay::offscreenSafePosX.max);
                    float minAllowedSpace = Constants::Gameplay::mailBoxSpacing.min + (Constants::Gameplay::mailBoxSpacing.max - Constants::Gameplay::mailBoxSpacing.min) * Constants::Gameplay::difficultyFactor;

                    if (minDist >= minAllowedSpace)
                    {
                        CreateSpawnPoint(SpawnPointType::MAILBOX, 0.06f);
                    }
                }
            }
        }
    }
    else
    {
        if (m_ActiveController && m_ActiveController->CheckButton(XINPUT_GAMEPAD_START, false))
        {
            StartGame();
        }
    }
}

void PaperGirl::StartGame()
{
    m_GameStarted = true;
}

void PaperGirl::ResetGame()
{
    //Init PaperGirl TRS
    m_MainCharacter->Reset();
    
    //Init Background
    m_Background->Reset();
    m_Background->scrollSpeed = 0.f;

    for (auto point : m_SpawnPoints)
    {
        point->Reset();
    }

    //Reset gameplay variables
    m_GameStarted = false;
    m_PressStartVisible = true;
    m_PressStartTimer = 0.f;

    m_BoostLevel = 1.f;
    m_NbCoins = 0;
    m_NbNewspaper = 0;
}

const Range& PaperGirl::GetScrollingRatio() const
{
    return m_Background->GetScrollingRatio();
}

float PaperGirl::GetScrollingSpeed() const 
{ 
    return m_Background->scrollSpeed; 
}

SpawnPoint* PaperGirl::CreateSpawnPoint(const SpawnPointType& _type, float _refPosY)
{
    //Try to reuse a free spawnPoint before creating a new one
    SpawnPoint* point = nullptr;
    if (!m_SpawnPointsPool.empty())
    {
        point = m_SpawnPointsPool.back();
        m_SpawnPointsPool.pop_back();
    }
    else
    {
        //Ultimately, create a new one
        point = new SpawnPoint(this);
    }

    m_SpawnPoints.push_back(point);

    //Init
    point->Init(_type, _refPosY);

    //Sort
    std::sort(m_SpawnPoints.begin(), m_SpawnPoints.end(), [](SpawnPoint* a, SpawnPoint* b) {
        return a->IsSpawned() && b->IsSpawned() && (a->GetPivotPos().y > b->GetPivotPos().y);
        });

    return point;
}

void PaperGirl::FreeSpawnPoint(SpawnPoint* _point)
{
    //Already in pool ?
    if (std::find(m_SpawnPointsPool.begin(), m_SpawnPointsPool.end(), _point) != m_SpawnPointsPool.end())
    {
        //Weird, element seems to be removed twice....
        assert(false);
        return;
    }

    auto it = std::find(m_SpawnPoints.begin(), m_SpawnPoints.end(), _point);
    if (it == m_SpawnPoints.end())
    {
        //Weird, element seems to come from an unknown location
        assert(false);
        return;
    }

    m_SpawnPointsPool.push_back(_point);
    m_SpawnPointsIt = m_SpawnPoints.erase(it);
}

float PaperGirl::GetClosestSpawnPoint(const SpawnPointType& _type, float _refPosX) const
{
    float minDist = 1000.f;

    for (auto point : m_SpawnPoints)
    {
        if (point->GetType() != _type || !point->IsSpawned())
            continue;

        float dist = abs(point->GetCenterPos().x - _refPosX);
        if (dist < minDist)
        {
            minDist = dist;
        }
    }

    return minDist;
}

bool PaperGirl::RequestSpawnPointActivation(SpawnPoint* _point, float _posX)
{
    if (_point->GetType() == SpawnPointType::GRANDMA)
    {
        //Get Closest GrandMa
        float minDist = GetClosestSpawnPoint(SpawnPointType::GRANDMA, _posX);
        float minSpaceBetweenGrandMa = Constants::Gameplay::grandMaSpacing.min + (Constants::Gameplay::grandMaSpacing.max - Constants::Gameplay::grandMaSpacing.min) * Constants::Gameplay::difficultyFactor;

        if (minDist < minSpaceBetweenGrandMa)
        {
            return false;
        }

        //Frequency
        float frequency = Constants::Gameplay::grandMaFrequency.min + (Constants::Gameplay::grandMaFrequency.max - Constants::Gameplay::grandMaFrequency.min) * Constants::Gameplay::difficultyFactor;
        if (Utils::Random(0, 1) > frequency)
        {
            return false;
        }

        return true;
    }

    return true;
}

bool PaperGirl::RequestSpawnPointRemoval(SpawnPoint* _point)
{
    if (!_point->IsAttachedToBackground())
    {
        FreeSpawnPoint(_point);
    }
    
    return true;
}

float PaperGirl::GetXInput() const
{
    if (m_ActiveController)
    {
        if (m_ActiveController->GetLeftThumbStickX() > STICK_THRESHOLD || m_ActiveController->GetLeftThumbStickX() < -STICK_THRESHOLD)
        {
            return m_ActiveController->GetLeftThumbStickX();
        }
        if (m_ActiveController->CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
        {
            return 1.f;
        }
        if (m_ActiveController->CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
        {
            return -1.f;
        }
    }
    return 0.f;
}

float PaperGirl::GetYInput() const
{
    if (m_ActiveController)
    {
        if (m_ActiveController->GetLeftThumbStickY() > STICK_THRESHOLD || m_ActiveController->GetLeftThumbStickY() < -STICK_THRESHOLD)
        {
            return m_ActiveController->GetLeftThumbStickY();
        }
        if (m_ActiveController->CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
        {
            return 1.f;
        }
        if (m_ActiveController->CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
        {
            return -1.f;
        }
    }
    return 0.f;
}

#ifdef _DEBUG
static float dbgY;

void PaperGirl::DrawDebug()
{
    dbgY = 0.97f;

    DrawDebugText("Difficulty", Constants::Gameplay::difficultyFactor);
}

void PaperGirl::DrawDebugText(const char* _text, float _value)
{
    char textBuffer[32];
    sprintf(textBuffer, "%s: %0.2f", _text, _value);
    App::Print(gameVp.GetX(0.9f), gameVp.GetY(dbgY), textBuffer, 1.0f, 0.0f, 1.0f, GLUT_BITMAP_HELVETICA_10);

    dbgY -= 0.025f;
}
#endif
