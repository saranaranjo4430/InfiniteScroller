//------------------------------------------------------------------------
#include <stdafx.h>

#include "..\MyGame\Utils\Utils.h"
#include "..\MyGame\Utils\GameSprite.h"

#include "Props.h"
#include "PaperGirl.h"
//------------------------------------------------------------------------

void Prop::Update(float _deltaTime)
{
    assert(sprite);
    sprite->Update(_deltaTime);
}

void Prop::Render()
{
    assert(sprite);
    sprite->Render();
}

void Prop::Shutdown()
{
    safedelete(sprite);
}

Vector2D Prop::GetSizeOnScreen() const
{ 
    assert(sprite);
    return sprite->GetSizeOnScreen();
}

//------------------------------------------------------------------------

void TheGirl::Init()
{
    sprite = new CGameSprite(".\\PaperGirl\\Data\\PaperGirl.bmp", 6, 5);

    //Init Animations
    sprite->CreateAnimation(ANIM_IDLE, 0.f, { 0 });

    sprite->CreateAnimation(ANIM_MOVE_SLOW, 0.4f, { 6,7,8,9,10,11 });
    sprite->CreateAnimation(ANIM_MOVE_NORMAL, 0.75f, { 6,7,8,9,10,11 });
    sprite->CreateAnimation(ANIM_MOVE_FAST, 0.25f, { 6,7,8,9,10,11 });

    sprite->CreateAnimation(ANIM_TRANSITION_START, 0.4f, { 0,1,2 });
    sprite->CreateAnimation(ANIM_TRANSITION_BOOST, 0.2f, { 24,25,26 });

    sprite->CreateAnimation(ANIM_ACTION_THROW, 1.5f, { 12,13,14,15,16,17 });
    sprite->CreateAnimation(ANIM_ACTION_FALL, 1.5f, { 18,19 });

    sprite->SetAnimation(ANIM_IDLE);

    //Init Collisions
    sprite->SetCollision(CollisionType::BOX, Vector2D(0.9f, 0.15f), Vector2D(-0.005f, -0.076f));
    sprite->AddFlag(GameSpriteFlags::DRAW_COLLISION);
}

void TheGirl::SetAnimation(const Animation& _anim)
{
    if (sprite->IsPlaying(_anim))
    {
        return;
    }

    bool loop = true;

    switch (_anim)
    {
    case TheGirl::ANIM_IDLE:
        //Don't interrupt transitions
        if (sprite->IsFinished(TheGirl::ANIM_TRANSITION_START) && sprite->IsFinished(TheGirl::ANIM_TRANSITION_BOOST))
        {
            sprite->SetAnimation(_anim, false);
        }
        break;

    case TheGirl::ANIM_MOVE_SLOW:
    case TheGirl::ANIM_MOVE_NORMAL:
    case TheGirl::ANIM_MOVE_FAST:
        //If PaperGirl is idle, need to wait for transition to be over
        if (sprite->IsPlaying(TheGirl::ANIM_IDLE))
        {
            sprite->SetAnimation(TheGirl::ANIM_TRANSITION_START, false);
            break;
        }

        if (sprite->IsPlaying(TheGirl::ANIM_TRANSITION_START) && !sprite->IsFinished(TheGirl::ANIM_TRANSITION_START))
        {
            break;
        }

        //If PaperGirl is moving fast, there's another specific transition
        if (_anim == TheGirl::ANIM_MOVE_FAST)
        {
            if (!sprite->IsPlaying(TheGirl::ANIM_TRANSITION_BOOST))
            {
                sprite->SetAnimation(TheGirl::ANIM_TRANSITION_BOOST, false);
                break;
            }

            if (sprite->IsFinished(TheGirl::ANIM_TRANSITION_BOOST))
            {
                sprite->SetAnimation(_anim);
            }
        }
        else
        {
            sprite->SetAnimation(_anim);
        }
        break;

    default:
        sprite->SetAnimation(_anim);
        break;
    }
}

void TheGirl::Reset()
{
    sprite->position = Vector2D(0.25f, 0.25f);
    sprite->scale = Vector2D(1.f, 1.f);
    sprite->SetAnimation(ANIM_IDLE);

    sprite->AddFlag(GameSpriteFlags::DRAW_COLLISION);
}

Vector2D TheGirl::GetSizeOnScreen() const
{ 
    return Prop::GetSizeOnScreen() * 0.85f;
}

//------------------------------------------------------------------------

void GrandMa::Init()
{
    //Init Sprite
    sprite = new CGameSprite(".\\PaperGirl\\Data\\GrandMa.bmp", 3, 1);

    //Init Animations
    sprite->CreateAnimation(ANIM_IDLE, 0.f, { 0 });
    sprite->CreateAnimation(ANIM_ASKING, 0.f, { 1 });
    sprite->CreateAnimation(ANIM_HAPPY, 0.f, { 2 });

    sprite->SetAnimation(ANIM_IDLE);

    sprite->AddFlag(GameSpriteFlags::DRAW_COLLISION);
}

void GrandMa::Reset()
{
    sprite->SetAnimation(ANIM_IDLE);
}

//------------------------------------------------------------------------

void RecycleBin::Init()
{
    //Init Sprite
    sprite = new CGameSprite(".\\PaperGirl\\Data\\RecycleBin.bmp", 1, 1);

    sprite->SetCollision(CollisionType::BOX, Vector2D(0.9f, 0.2f), Vector2D(0.0f, -0.05f));
    sprite->AddFlag(GameSpriteFlags::DRAW_COLLISION);
}

//------------------------------------------------------------------------

void Mailbox::Init()
{
    //Init Sprite
    sprite = new CGameSprite(".\\PaperGirl\\Data\\Mailbox.bmp", 1, 1);

    sprite->SetCollision(CollisionType::BOX, Vector2D(1.f, 0.5f), Vector2D(0.0f, -0.03f));
    sprite->AddFlag(GameSpriteFlags::DRAW_COLLISION);
}

