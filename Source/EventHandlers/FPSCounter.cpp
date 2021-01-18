#include "FPSCounter.hpp"

FPSCounter::FPSCounter(double* t, float fpsUpdateTime)
{
    this->t = t;
    this->fpsUpdateTime = fpsUpdateTime;
}

float FPSCounter::GetFPS()
{
    return pastFPS;
}

void FPSCounter::Start()
{
    fpsPrevTime = 0;
}

void FPSCounter::FlameUpdate()
{
    double deltaTime = *t - fpsPrevTime;
    fpsFrameCount++;

    if(deltaTime >= fpsUpdateTime)
    {
        pastFPS = fpsFrameCount / deltaTime;
        fpsFrameCount = 0;
        fpsPrevTime = *t;
    }
}