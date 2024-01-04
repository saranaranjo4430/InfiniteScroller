#pragma once

#include "..\MyGame\Utils\GameViewport.h"
#include "..\MyGame\Utils\Vectors.h"

class CGameSprite;

class Prop
{
public:
    Prop() {};
    virtual ~Prop() {};

    virtual void Init() = 0;
    virtual void Update(float _deltaTime);
    virtual void Render();
    virtual void Shutdown();

    virtual void Reset() {};

    CGameSprite* sprite;
};

class PropLogic
{

};

class TheGirl : public Prop
{
public:
    TheGirl() {};
    ~TheGirl() {};

    enum Animation
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

    void Init();
    void SetAnimation(const Animation& _anim);
    void Reset();
};

class GrandMa : public Prop
{
public:
    GrandMa() {};
    ~GrandMa() {};

    enum Animation
    {
        ANIM_IDLE = 0,
        ANIM_ASKING,
        ANIM_HAPPY,
    };

    void Init();
    void SetAnimation(const Animation& _anim);
    void Reset();
};

class RecycleBin : public Prop
{
public:
    RecycleBin() {};
    ~RecycleBin() {};

    void Init();
};

class Mailbox : public Prop
{
public:
    Mailbox() {};
    ~Mailbox() {};

    void Init();
};
