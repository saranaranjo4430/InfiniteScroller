#pragma once

namespace Constants
{
    namespace Interface
    {
        static float pressStartPulseDuration = 0.4f;
    }

    namespace Background
    {
        static float scale = 1.25f;
    }

    namespace Gameplay
    {
        static float difficultyFactor = 0.0f;                   //0.f easy, 1.f hell on earth
        static float difficultyAccSpeed = 0.001f;

        static Range roadHeight = Range(0.095f, 0.32f);

        static float scrollAccSpeed = 0.005f;
        static Range scrollSpeed = Range(0.08f, 0.20f);         //Min & Max depends on difficulty factor

        static Range grandMaSpacing = Range(0.25f, 0.5f);       //Min & Max depends on difficulty factor
        static Range grandMaFrequency = Range(0.95f, 0.5f);     //Min & Max depends on difficulty factor

        static Range recycleBinSpacing = Range(0.4f, 0.2f);     //Min & Max depends on difficulty factor
        static Range recycleBinFrequency = Range(0.3f, 0.8f);   //Min & Max depends on difficulty factor

        static Range mailBoxSpacing = Range(0.6f, 0.5f);        //Min & Max depends on difficulty factor
        static Range mailBoxFrequency = Range(0.075f, 0.3f);      //Min & Max depends on difficulty factor

        static Range offscreenSafePosX = Range(-0.1f, 1.1f);
    }

    namespace TheGirl
    {
        static float fastSpeed = 0.1f;
        static float slowSpeed = 0.15f;
        static float verticalSpeed = 0.1f;

        static float textureMargin = 0.425f;
    }
}
