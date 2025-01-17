//------------------------------------------------------------------------
#include "stdafx.h"

#include <chrono>
#include <random>

#include "..\app\app.h"
#include "..\app\main.h"

#include "Utils.h"
//------------------------------------------------------------------------

namespace Utils
{
    // !! Coordinates must be in Pixels !!
    void DrawPolygon(float _sx, float _sy, float _ex, float _ey, const Color& _col)
    {
#if APP_USE_VIRTUAL_RES		
        APP_VIRTUAL_TO_NATIVE_COORDS(_sx, _sy);
        APP_VIRTUAL_TO_NATIVE_COORDS(_ex, _ey);
#endif
        glBegin(GL_POLYGON);
        glColor3f(_col.r, _col.g, _col.b); // Yellow
        glVertex2f(_sx, _sy);
        glVertex2f(_ex, _sy);
        glVertex2f(_ex, _ey);
        glVertex2f(_sx, _ey);
        glEnd();
    }

    // !! Coordinates must be in Pixels !!
    void DrawLine(float _sx, float _sy, float _ex, float _ey, const Color& _col)
    {
        App::DrawLine(_sx, _sy, _ex, _ey, _col.r, _col.g, _col.b);
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

    float Clamp(float value, float min, float max)
    {
        return (value < min) ? min : (value > max) ? max : value;
    }

    float ClampMin(float value, float min)
    {
        return (value < min) ? min : value;
    }

    float ClampMax(float value, float max)
    {
        return (value > max) ? max : value;
    }

    float DegToRad(float degrees)
    {
        return degrees * PI / 180.f;
    }

    float RadToDeg(float radians)
    {
        return radians * 180.f / PI;
    }

    float Random(float fMin, float fMax)
    {
        // initialize the random number generator with time-dependent seed
        uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::seed_seq seed{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
        auto urbg = std::mt19937{ seed };
        auto norm = std::uniform_real_distribution<float>{ fMin, fMax };
        return norm(urbg);
    }

    float Epsilon()
    {
        return std::numeric_limits<float>::epsilon();
    }
}
