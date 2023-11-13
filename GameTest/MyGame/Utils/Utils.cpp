//------------------------------------------------------------------------
#include "stdafx.h"

#include "..\app\app.h"
#include "..\app\main.h"

#include "Utils.h"
//------------------------------------------------------------------------

namespace Utils
{
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
}
