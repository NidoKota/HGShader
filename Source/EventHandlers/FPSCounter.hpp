#include <iostream>
#include <math.h>
#include <time.h>
#include "../Library/Defines.hpp"
#include "../Library/Event.hpp"

//FPS数を計る
struct FPSCounter : public EventHandler
{
    private : 
    int fpsFrameCount;
    float fpsUpdateTime;
    float pastFPS;
    double fpsPrevTime;
    double* t;

    void FlameUpdate() override;

    public : 
    //経過時間が入った変数のポインタと、何秒ごとにFPS数を更新するかを登録
    FPSCounter(double* t, float fpsUpdateTime);
    //再度初期化処理を行う
    void Start() override;
    //FPSを取得
    float GetFPS();
};