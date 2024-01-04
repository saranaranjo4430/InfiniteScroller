#pragma once

#include "..\MyGame\Utils\GameViewport.h"
#include "..\MyGame\Utils\Vectors.h"

#include "Props.h"

class SpawnPoint;

enum SpawnPointType
{
    GRANDMA = 0,
    RECYCLEBIN,
    MAILBOX
};

class SpawnPointOwner
{
public:
    virtual bool RequestSpawnPointActivation(SpawnPoint* _point, float _posX) = 0;
    virtual bool RequestSpawnPointRemoval(SpawnPoint* _point) = 0;
};

class SpawnPoint : public PropLogic
{
public:
    SpawnPoint(SpawnPointOwner* _owner);
    ~SpawnPoint();

    void Init(const SpawnPointType& _type, float _refPosY);
    void Update(float _deltaTime);
    void Render();
    void Shutdown();

    void Reset();

    SpawnPointType GetType() const { return m_Type; }
    
    void AttachToBackground(float _scrollingRatio);
    bool IsAttachedToBackground() const;
    float GetBackgroundRatio() const { return m_ScrollingRatio; }

    bool IsSpawned() const { return (m_SpawnedObject != nullptr); }
    Vector2D GetCenterPos() const;
    Vector2D GetPivotPos() const;

protected:
    void SpawnObject(float _posX);
    void UnspawnObject();

    bool IsRatioInView() const;

private:
    SpawnPointOwner* m_Owner;
    SpawnPointType m_Type;
    float m_ScrollingRatio;
    float m_RefPosY;

    Prop* m_SpawnedObject = nullptr;
    bool m_SpawnAsked = false;
};