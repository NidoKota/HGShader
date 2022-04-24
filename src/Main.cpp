#include "Main.hpp"

int main()
{
    MainLoop(Start, FlameUpdate);
    exit(EXIT_SUCCESS);
}

static Delegate flameUpdateDelegate = Delegate();

void Start()
{
    HgOpen(WINDOW_SIZE, WINDOW_SIZE);

    fpsCounterPtr = GetFPSCounterPtr(&flameUpdateDelegate);
    tcpManagerPtr = GetTCPManagerPtr(&flameUpdateDelegate);
    inputManagerPtr = GetInputManagerPtr(&flameUpdateDelegate);
    shaderDisplayerPtr = GetShaderDisplayerPtr(&flameUpdateDelegate);
    infoDisplayerPtr = GetInfoDisplayPtr(&flameUpdateDelegate);
}

void FlameUpdate()
{
    flameUpdateDelegate.Invoke();

    //InputManagerから取得したキー入力によって何をするのか決める
    int shaderIndex = shaderDisplayerPtr->GetShaderFuncsIndex();
    int shaderSize = shaderDisplayerPtr->GetShaderFuncsSize();
    if(inputManagerPtr->GetKeyDown(HG_U_ARROW)) shaderDisplayerPtr->IncreasePixCount();
    if(inputManagerPtr->GetKeyDown(HG_D_ARROW)) shaderDisplayerPtr->DecreasePixCount();
    if(inputManagerPtr->GetKeyDown(HG_R_ARROW)) shaderDisplayerPtr->SetShaderFuncsIndex(shaderIndex + 1 > shaderSize - 1 ? 0 : shaderIndex + 1);
    if(inputManagerPtr->GetKeyDown(HG_L_ARROW)) shaderDisplayerPtr->SetShaderFuncsIndex(shaderIndex - 1 < 0 ? shaderSize - 1 : shaderIndex - 1);
    if(inputManagerPtr->GetKeyDown('q')) isPlaying = false;
    if(inputManagerPtr->GetKeyDown('f')) isFPSLimitEnabled = !isFPSLimitEnabled;
    if(inputManagerPtr->GetKeyDown(' ')) infoIndex = (infoIndex + 1 > 3 ? 0 : infoIndex + 1);
    if(inputManagerPtr->GetKeyDown(',')) shaderDisplayerPtr->SetShaderSpeed(clamp((float)shaderDisplayerPtr->GetShaderSpeed() - 0.25f, 0.f, 10.f));
    if(inputManagerPtr->GetKeyDown('.')) shaderDisplayerPtr->SetShaderSpeed(clamp((float)shaderDisplayerPtr->GetShaderSpeed() + 0.25f, 0.f, 10.f));
    if(inputManagerPtr->GetKeyDown('r'))
    {
        totalTime = 0;
        shaderDisplayerPtr->Start();
        fpsCounterPtr->Start();
    }
}