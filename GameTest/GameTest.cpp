//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
#include "app\main.h"
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------

void DrawPolygon(float sx, float sy, float ex, float ey, float r, float g, float b)
{
#if APP_USE_VIRTUAL_RES		
    APP_VIRTUAL_TO_NATIVE_COORDS(sx, sy);
    APP_VIRTUAL_TO_NATIVE_COORDS(ex, ey);
#endif
    glBegin(GL_POLYGON);
    glColor3f(r, g, b); // Yellow
    glVertex2f(sx, sy);
    glVertex2f(ex, sy);
    glVertex2f(ex, ey);
    glVertex2f(sx, ey);
    glEnd();
}

const CController* GetFirstActiveController()
{
    CSimpleControllers& instance = CSimpleControllers::GetInstance();

    for (int i = 0; i < MAX_CONTROLLERS; i++)
    {
        if (instance.GetController(i).IsConnected() && instance.GetController(i).IsActive())
        {
            return &instance.GetController(i);
        }
    }

    return NULL;
}

class Vector2D
{
public:
	Vector2D() : x(0.f), y(0.f) {};
	Vector2D(float _x, float _y) : x(_x), y(_y) {};

	float x;
	float y;
};

//The Game Viewport provides helper functions so that a predefined aspect-ratio is ensured
//It creates borders on the left/right of the screen or on the top/bottom depending on the application window size
class GameViewport
{
public:
	GameViewport(int _width, int _height) 
	{ 
        m_VpWidth = _width;
        m_VpHeight = _height;
		m_VpRatio = (float)m_VpWidth / (float)m_VpHeight;
	}

	float getRatio() const { return m_VpRatio; }

	//Converts a viewport percentage to an app window coordinates in pixels
    float getX(float _percent) const
    {
		float appWindowRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
		if (appWindowRatio > m_VpRatio)
		{
			//App Window is larger than the Game Viewport
			float vpSizeWithinAppWindow = (m_VpRatio / appWindowRatio) * APP_INIT_WINDOW_WIDTH;
			float vpOffset = (APP_INIT_WINDOW_WIDTH - vpSizeWithinAppWindow) *0.5f;
			float x = vpOffset + (vpSizeWithinAppWindow * _percent);
			return x;
		}
		
		return APP_INIT_WINDOW_WIDTH * _percent;
    }

	//Converts a viewport percentage to an app window coordinates in pixels
    float getY(float _percent) const
    {
        float appWindowRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        if (appWindowRatio < m_VpRatio)
        {
			//App Window is smaller than the Game Viewport
            float vpSizeWithinAppWindow = (appWindowRatio / m_VpRatio) * APP_INIT_WINDOW_HEIGHT;
            float vpOffset = (APP_INIT_WINDOW_HEIGHT - vpSizeWithinAppWindow) * 0.5f;
            float y = vpOffset + (vpSizeWithinAppWindow * _percent);
			return y;
        }
		
		return APP_INIT_WINDOW_HEIGHT * _percent;
    }

	//Converts a viewport percentage size to an app window size in pixels
	float getWidth(float _percent) const
	{
		float min = getX(0.f);
		float max = getX(1.f);
		float size = (max - min) * _percent;
		return size;
	}

	//Converts a viewport percentage size to an app window size in pixels
    float getHeight(float _percent) const
    {
		float min = getY(0.f);
		float max = getY(1.f);
        float size = (max - min) * _percent;
        return size;
    }

	//Converts a viewport scale to an app window scale
	float getScaleX(float _scale) const
	{
		float ratio = getWidth(1.f) / (float)m_VpHeight;
		ratio *= _scale;
		ratio /= m_VpRatio;
		return ratio;
	}

	//Converts a viewport scale to an app window scale
    float getScaleY(float _scale) const
    {
		float ratio = getHeight(1.f) / (float)m_VpHeight;
        ratio *= _scale;
		return ratio;
    }

private:
	float m_VpRatio = 0.f;
    int m_VpWidth = 0;
    int m_VpHeight = 0;
};

class CSprite
{
public:
	CSprite(const GameViewport& _vp, const char* _fileName, int _columns, int _rows) :
		m_Vp(_vp)
	{
		m_BaseSprite = App::CreateSprite(_fileName, _columns, _rows);
	}

	~CSprite()
	{
		delete m_BaseSprite;
	}

	void Update(float _deltaTime)
	{
		m_BaseSprite->Update(_deltaTime);

		m_BaseSprite->SetPosition(m_Vp.getX(position.x), m_Vp.getY(position.y));
		m_BaseSprite->SetScaleX(m_Vp.getScaleX(scale.x));
		m_BaseSprite->SetScaleY(m_Vp.getScaleY(scale.y));
		m_BaseSprite->SetAngle(angle);
	}

	void Draw()
	{
		m_BaseSprite->Draw();
	}

	void SetAnimation(int id)
	{
		m_BaseSprite->SetAnimation(id);
	}

    void CreateAnimation(unsigned int id, float speed, const std::vector<int>& frames)
	{
		m_BaseSprite->CreateAnimation(id, speed, frames);
	}

public:
    Vector2D position = Vector2D(0.f, 0.f);
    Vector2D scale = Vector2D(0.f, 0.f);
    float angle = 0.f;

private:
	const GameViewport& m_Vp;
	CSimpleSprite* m_BaseSprite;
};


//------------------------------------------------------------------------
// Example data....
//------------------------------------------------------------------------
CSprite* testSprite;
enum
{
    ANIM_FORWARDS,
    ANIM_BACKWARDS,
    ANIM_LEFT,
    ANIM_RIGHT,
};

GameViewport viewport = GameViewport(432, 768);
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	testSprite = new CSprite(viewport, ".\\TestData\\Test.bmp", 8, 4);
	testSprite->position = Vector2D(0.f, 0.5f);
	testSprite->scale = Vector2D(1.f, 1.f);

	float speed = 1.0f / 15.0f;
	testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	
	App::LoadSound(".\\TestData\\Test.wav");
	//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
    testSprite->Update(deltaTime);

	const CController* pController = GetFirstActiveController();
	if (pController)
	{
		float HSpeed = 0.01f;
		float VSpeed = HSpeed * viewport.getRatio();

		if ((pController->GetLeftThumbStickX() > 0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
		{
			testSprite->SetAnimation(ANIM_RIGHT);
			testSprite->position.x += HSpeed;
		}
		if ((pController->GetLeftThumbStickX() < -0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
		{
			testSprite->SetAnimation(ANIM_LEFT);
			testSprite->position.x -= HSpeed;
		}
		if ((pController->GetLeftThumbStickY() > 0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
		{
			testSprite->SetAnimation(ANIM_FORWARDS);
			testSprite->position.y += VSpeed;
		}
		if ((pController->GetLeftThumbStickY() < -0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
		{
			testSprite->SetAnimation(ANIM_BACKWARDS);
			testSprite->position.y -= VSpeed;
		}
		if (pController->CheckButton(XINPUT_GAMEPAD_LEFT_SHOULDER, false))
		{
			testSprite->scale.x += HSpeed;
		}
		if (pController->CheckButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, false))
		{
			testSprite->scale.x -= VSpeed;
		}
		if (pController->GetLeftTrigger())
		{
			testSprite->angle += 0.1f * pController->GetLeftTrigger();
		}
		if (pController->GetRightTrigger())
		{
			testSprite->angle -= 0.1f * pController->GetRightTrigger();
		}

		//------------------------------------------------------------------------
		// Sample Sound.
		//------------------------------------------------------------------------
		if (pController->CheckButton(XINPUT_GAMEPAD_B, true))
		{
			App::PlaySound(".\\TestData\\Test.wav");
		}
	}
	else
	{
		testSprite->SetAnimation(-1);
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	DrawPolygon(viewport.getX(0.f), viewport.getY(0.f), viewport.getX(1.f), viewport.getY(1.f), 1, 0, 0);

	//------------------------------------------------------------------------
	// Example Sprite Code....
	testSprite->Draw();
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(100, 100, "Sample Text");

	//------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
	static float a = 0.0f;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	a += 0.1f;
	for (int i = 0; i < 20; i++)
	{

		float sx = 200 + sinf(a + i * 0.1f)*60.0f;
		float sy = 200 + cosf(a + i * 0.1f)*60.0f;
		float ex = 700 - sinf(a + i * 0.1f)*60.0f;
		float ey = 700 - cosf(a + i * 0.1f)*60.0f;
		g = (float)i / 20.0f;
		b = (float)i / 20.0f;
		App::DrawLine(sx, sy, ex, ey,r,g,b);
	}
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete testSprite;
	//------------------------------------------------------------------------
}

