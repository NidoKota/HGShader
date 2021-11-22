#include "Main.hpp"

std::chrono::system_clock::time_point sT, eT;
std::string updatePixSS;
std::vector<zigsimdata> zsds = std::vector<zigsimdata>();
double t = 0;                 
double prevT = 0;                 
double deltaTime = 0;
double sleepTime = 0;
int infoIndex = 1;
bool isFPSLimitEnabled = true;
bool isPraying = true;

Delegate flameUpdateDelegate = Delegate();
FPSCounter fpsCounter = FPSCounter(&t, 0.5f);
TCPManager tcpManager = TCPManager(8888, 1024, OnMessage, OnChangeConnect);
InputManager inputManager = InputManager(HG_U_ARROW, HG_D_ARROW, HG_R_ARROW, HG_L_ARROW, 'q', 'f', 'r', ' ', ',', '.');
ShaderDisplayer shaderDisplayer = ShaderDisplayer(&t, &zsds, RainbowCircleShader, HeartShader, WaveShader, RotationShapesShader, LoadingShader, SpaceShader, BlockShader, PhoneShader);
DoubleLayerDisplayer infoDisplayer = DoubleLayerDisplayer(InfoDisplay);

int main()
{
    using namespace std::chrono;
    
    HgOpen(WINDOW_SIZE, WINDOW_SIZE);
    //イベントの購読
    fpsCounter.Subscribe(flameUpdateDelegate);
    tcpManager.Subscribe(flameUpdateDelegate);
    inputManager.Subscribe(flameUpdateDelegate);
    shaderDisplayer.Subscribe(flameUpdateDelegate);
    infoDisplayer.Subscribe(flameUpdateDelegate);

    while(isPraying)
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
    if(inputManager.GetKeyDown('q')) isPraying = false;
    if(inputManager.GetKeyDown('f')) isFPSLimitEnabled = !isFPSLimitEnabled;
    if(inputManager.GetKeyDown(' ')) infoIndex = (infoIndex + 1 > 3 ? 0 : infoIndex + 1);
    if(inputManager.GetKeyDown(',')) shaderDisplayer.SetShaderSpeed(clamp((float)shaderDisplayer.GetShaderSpeed() - 0.25f, 0.f, 10.f));
    if(inputManager.GetKeyDown('.')) shaderDisplayer.SetShaderSpeed(clamp((float)shaderDisplayer.GetShaderSpeed() + 0.25f, 0.f, 10.f));
    if(inputManager.GetKeyDown('r'))
    {
        t = 0;
        shaderDisplayer.Start();
        fpsCounter.Start();
    }
}

void OnMessage(int id, int index, char* buf)
{
    //メッセージを受け取ったらJsonの解析をする
    zsds[index].Parse(buf);
}

void OnChangeConnect()
{
    for (int i = 0; zsds.size() > tcpManager.GetConnectCount() && i < zsds.size() - tcpManager.GetConnectCount(); i++) zsds.pop_back();
    for (int i = 0; zsds.size() < tcpManager.GetConnectCount() && i < tcpManager.GetConnectCount() - zsds.size(); i++) zsds.push_back(zigsimdata());
}

void InfoDisplay(int layerId)
{    
    //画面に表示する情報
    if(infoIndex != 0)
    {
        HgWSetFont(layerId, HG_HB, 15);
        HgWSetColor(layerId, vec3(0.5).ToHGColor());
        HgWSetFillColor(layerId, vec3(0.5).ToHGColor());
        HgWText(layerId, 0, WINDOW_SIZE - 15, "FPS : %5.1f FPSLimit : %s", fpsCounter.GetFPS(), isFPSLimitEnabled ? "ON" : "OFF");
        HgWText(layerId, 0, WINDOW_SIZE - 30, "Time : %3.1f ShaderSpeed : %3.2f", t, shaderDisplayer.GetShaderSpeed());
    }
    if(infoIndex == 1)
    {
        HgWText(layerId, 0, WINDOW_SIZE - 45, "SleepTime : %1.3f DeltaTime : %1.3f", sleepTime, deltaTime);
        HgWText(layerId, 0, WINDOW_SIZE - 60, "Window : %dx%d PixCount : %dx%d", WINDOW_SIZE, WINDOW_SIZE, shaderDisplayer.GetPixCount(), shaderDisplayer.GetPixCount());
        HgWText(layerId, 0, WINDOW_SIZE - 75, "InputKeys : %s", std::bitset<10>(inputManager.GetAnyKey()).to_string().c_str());
    }
    if(infoIndex == 2)
    {
        //上から、CalcColorsTime、RenderColorsTime、DeltaTime、SleepTimeのグラフを表示
        HgWSetFont(layerId, HG_HB, 10);
        HgWText(layerId, 0, WINDOW_SIZE - 43, "CC");
        HgWText(layerId, 0, WINDOW_SIZE - 51, "RC");
        HgWText(layerId, 0, WINDOW_SIZE - 59, "DT");
        HgWText(layerId, 0, WINDOW_SIZE - 67, "ST");
        HgWBoxFill(layerId, 15, WINDOW_SIZE - 43 + 6, shaderDisplayer.GetCalcColorsDeltaTime() * 1000, 2.5f, false);
        HgWBoxFill(layerId, 15, WINDOW_SIZE - 51 + 6, shaderDisplayer.GetRenderColorsDeltaTime() * 1000, 2.5f, false);
        HgWBoxFill(layerId, 15, WINDOW_SIZE - 59 + 6, deltaTime * 1000, 2.5f, false);
        HgWBoxFill(layerId, 15, WINDOW_SIZE - 67 + 6, sleepTime * 1000, 2.5f, false);
        //グラフの縦線を描画
        for (int i = 0; i <= 10; i++) HgWBoxFill(layerId, 15 + i * 10, WINDOW_SIZE - 65, 1.5f, 32, false);
    }
    if(infoIndex == 3)
    {
        HgWText(layerId, 0, WINDOW_SIZE - 45, "Connect : %d Port : %d", tcpManager.GetConnectCount(), tcpManager.GetPort());
        for (int i = 0; i < zsds.size(); i++) HgWText(layerId, 0, WINDOW_SIZE - 60 - (15 * i), "Name : %.25s", zsds[i]._device.name);
    }
}