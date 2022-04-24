#include "MainLoop.hpp"

bool isFPSLimitEnabled = true;
bool isPlaying = true;
double deltaTime = 0;
double sleepTime = 0;
double totalTime = 0;
int infoIndex = 1;

void MainLoop(OnStart onStart, OnFlameUpdate onFlameUpdate)
{
    onStart();

    using namespace std::chrono;
    system_clock::time_point sT;

    while(isPlaying)
    {
        //Update開始前の時間を取得
        sT = system_clock::now();

        onFlameUpdate();

        //DeltaTimeを計算
        deltaTime = duration_cast<milliseconds>(system_clock::now() - sT).count() / 1000.f;

        //FPS数制限が設定されている時
        if(isFPSLimitEnabled)
        {
            //TARGET_FPSが実現した時のdeltaTimeと比べて、Threadを止める時間を決める
            double targetDeltaTime = 1. / (double)TARGET_FPS;
            sleepTime = targetDeltaTime - deltaTime;
            sleepTime = 0 > sleepTime ? 0 : targetDeltaTime < sleepTime ? targetDeltaTime : sleepTime;
            HgSleep(sleepTime);
        }
        else sleepTime = 0;

        totalTime += deltaTime + sleepTime;
    }
}