#include "Main.hpp"

std::chrono::system_clock::time_point sT, eT;
std::string updatePixSS;
double t = 0;                 
double prevT = 0;                 
double deltaTime = 0;
double sleepTime = 0;
bool isFPSLimitEnabled = true;
bool isPlaying = true;

Delegate flameUpdateDelegate = Delegate();

FPSCounter fpsCounter = FPSCounter(&t, 0.5f);
InputManager inputManager = InputManager(HG_U_ARROW, HG_D_ARROW, HG_R_ARROW, HG_L_ARROW, 'q', 'f', 'r', ' ', ',', '.');
ShaderDisplayer shaderDisplayer = ShaderDisplayer(&t, RainbowCircleShader, HeartShader, WaveShader, RotationShapesShader, LoadingShader, SpaceShader, BlockShader);
DoubleLayerDisplayer infoDisplayer = DoubleLayerDisplayer([](int layerID)
{
    //画面に表示する情報
    HgWSetFont(layerID, HG_HB, 15);
    HgWSetColor(layerID, vec3(0.5).ToHGColor());
    HgWSetFillColor(layerID, vec3(0.5).ToHGColor());
    HgWText(layerID, 0, WINDOW_SIZE - 15, "FPS : %5.1f FPSLimit : %s", fpsCounter.GetFPS(), isFPSLimitEnabled ? "ON" : "OFF");
    HgWText(layerID, 0, WINDOW_SIZE - 30, "Time : %3.1f ShaderSpeed : %3.2f", t, shaderDisplayer.GetShaderSpeed());
    HgWText(layerID, 0, WINDOW_SIZE - 45, "SleepTime : %1.3f DeltaTime : %1.3f", sleepTime, deltaTime);
    HgWText(layerID, 0, WINDOW_SIZE - 60, "Window : %dx%d PixCount : %dx%d", WINDOW_SIZE, WINDOW_SIZE, shaderDisplayer.GetPixCount(), shaderDisplayer.GetPixCount());
    HgWText(layerID, 0, WINDOW_SIZE - 75, "InputKeys : %s", std::bitset<10>(inputManager.GetAnyKey()).to_string().c_str());

    //上から、PixUpdateTime、PixRenderTime、DeltaTime、SleepTimeのグラフを表示
    HgWSetFont(layerID, HG_HB, 10);
    HgWText(layerID, 0, WINDOW_SIZE - 88, "UP");
    HgWText(layerID, 0, WINDOW_SIZE - 96, "RT");
    HgWText(layerID, 0, WINDOW_SIZE - 104, "DT");
    HgWText(layerID, 0, WINDOW_SIZE - 112, "ST");
    HgWBoxFill(layerID, 15, WINDOW_SIZE - 88 + 6, shaderDisplayer.GetPixUpdateDeltaTime() * 1000, 2.5f, false);
    HgWBoxFill(layerID, 15, WINDOW_SIZE - 96 + 6, shaderDisplayer.GetPixRenderDeltaTime() * 1000, 2.5f, false);
    HgWBoxFill(layerID, 15, WINDOW_SIZE - 104 + 6, deltaTime * 1000, 2.5f, false);
    HgWBoxFill(layerID, 15, WINDOW_SIZE - 112 + 6, sleepTime * 1000, 2.5f, false);
    //グラフの縦線を描画
    for (int i = 0; i <= 10; i++) HgWBoxFill(layerID, 15 + i * 10, WINDOW_SIZE - 110, 1.5f, 32, false);
});

int main()
{
    using namespace std::chrono;

    HgOpen(WINDOW_SIZE, WINDOW_SIZE);
    
    //イベントの購読
    fpsCounter.Subscribe(flameUpdateDelegate);
    inputManager.Subscribe(flameUpdateDelegate);
    shaderDisplayer.Subscribe(flameUpdateDelegate);
    infoDisplayer.Subscribe(flameUpdateDelegate);

    while(isPlaying)
    {
        //FlameUpdate開始前の時間を取得
        sT = system_clock::now();

        //FlameUpdateを呼ぶ
        flameUpdateDelegate.Invoke();
        MainFlameUpdate();
        
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

        t += deltaTime + sleepTime;
    }

    return 0;
}

void MainFlameUpdate()
{
    //InputManagerから取得したキー入力によって何をするのか決める
    int shaderIndex = shaderDisplayer.GetShaderFuncsIndex();
    int shaderSize = shaderDisplayer.GetShaderFuncsSize();
    if(inputManager.GetKeyDown(HG_U_ARROW)) shaderDisplayer.IncreasePixCount();
    if(inputManager.GetKeyDown(HG_D_ARROW)) shaderDisplayer.DecreasePixCount();
    if(inputManager.GetKeyDown(HG_R_ARROW)) shaderDisplayer.SetShaderFuncsIndex(shaderIndex + 1 > shaderSize - 1 ? 0 : shaderIndex + 1);
    if(inputManager.GetKeyDown(HG_L_ARROW)) shaderDisplayer.SetShaderFuncsIndex(shaderIndex - 1 < 0 ? shaderSize - 1 : shaderIndex - 1);
    if(inputManager.GetKeyDown('q')) isPlaying = false;
    if(inputManager.GetKeyDown('f')) isFPSLimitEnabled = !isFPSLimitEnabled;
    if(inputManager.GetKeyDown(' ')) infoDisplayer.SetActive(!infoDisplayer.GetActive());
    if(inputManager.GetKeyDown(',')) shaderDisplayer.SetShaderSpeed(clamp((float)shaderDisplayer.GetShaderSpeed() - 0.25f, 0.f, 10.f));
    if(inputManager.GetKeyDown('.')) shaderDisplayer.SetShaderSpeed(clamp((float)shaderDisplayer.GetShaderSpeed() + 0.25f, 0.f, 10.f));
    if(inputManager.GetKeyDown('r'))
    {
        t = 0;
        shaderDisplayer.Start();
        fpsCounter.Start();
    }
}