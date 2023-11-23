#pragma once

//The Game Viewport provides helper functions so that a predefined aspect-ratio is ensured
//It creates borders on the left/right of the screen or on the top/bottom depending on the application window size

enum ViewportFlags
{
    DRAW_VIEWPORT = (1 << 0),
    DRAW_BORDERS = (1 << 1),
};

class GameViewport
{
public:
    GameViewport() {};

    void Init(int width, int height);
    void Update(float _deltaTime) {};
    void Render();
    void Shutdown() {};
    
    inline float GetRatio() const { return m_VpRatio; }
    
    inline void AddFlag(ViewportFlags flags) { m_Flags |= flags; }
    inline void RemoveFlag(ViewportFlags flags) { m_Flags &= ~flags; }
    inline bool HasFlag(ViewportFlags flags) { return (m_Flags & flags); }

    //Converts a viewport percentage to an app window coordinates in pixels
    float GetX(float _percent) const;

    //Converts a viewport percentage to an app window coordinates in pixels
    float GetY(float _percent) const;

    //Converts a viewport percentage size to an app window size in pixels
    float GetWidth(float _percent) const;

    //Converts a viewport percentage size to an app window size in pixels
    float GetHeight(float _percent) const;

    //Converts a viewport scale to an app window scale
    float GetScaleX(float _scale) const;

    //Converts a viewport scale to an app window scale
    float GetScaleY(float _scale) const;

private:
    float m_VpRatio = 0.f;
    int m_VpWidth = 0;
    int m_VpHeight = 0;
    int m_Flags = 0;
    unsigned int bgTextureID; // Declare the texture ID for the background ME 
};
