#include "MainComponent.hpp"

FPSCounter* fpsCounterPtr;
TCPManager* tcpManagerPtr;
InputManager* inputManagerPtr;
ShaderDisplayer* shaderDisplayerPtr;
DoubleLayerDisplayer* infoDisplayerPtr;

static std::vector<zigsimdata> zsds = std::vector<zigsimdata>();

FPSCounter* GetFPSCounterPtr(Delegate* flameUpdateDelegatePtr)
{
    FPSCounter* result = new FPSCounter(&totalTime, 0.5f);
    result->Subscribe(*flameUpdateDelegatePtr);
    return result;
}

TCPManager* GetTCPManagerPtr(Delegate* flameUpdateDelegatePtr)
{
    //メッセージを受け取ったらJsonの解析をする
    void (*OnMessage)(int, int, char*) = [](int id, int index, char* buf)
    {
        zsds[index].Parse(buf);
    };

    void (*OnChangeConnect)() = []()
    {
        for (int i = 0; zsds.size() > tcpManagerPtr->GetConnectCount() && i < zsds.size() - tcpManagerPtr->GetConnectCount(); i++) zsds.pop_back();
        for (int i = 0; zsds.size() < tcpManagerPtr->GetConnectCount() && i < tcpManagerPtr->GetConnectCount() - zsds.size(); i++) zsds.push_back(zigsimdata());
    };
    
    TCPManager* result = new TCPManager(8888, 1024, OnMessage, OnChangeConnect);
    result->Subscribe(*flameUpdateDelegatePtr);
    return result;
}

InputManager* GetInputManagerPtr(Delegate* flameUpdateDelegatePtr)
{
    InputManager* result = new InputManager(HG_U_ARROW, HG_D_ARROW, HG_R_ARROW, HG_L_ARROW, 'q', 'f', 'r', ' ', ',', '.');
    result->Subscribe(*flameUpdateDelegatePtr);
    return result;
}

ShaderDisplayer* GetShaderDisplayerPtr(Delegate* flameUpdateDelegatePtr)
{
    ShaderDisplayer* result = new ShaderDisplayer(&totalTime, &zsds, RainbowCircleShader, HeartShader, WaveShader, RotationShapesShader, LoadingShader, SpaceShader, BlockShader, PhoneShader);
    result->Subscribe(*flameUpdateDelegatePtr);
    return result;
}

DoubleLayerDisplayer* GetInfoDisplayPtr(Delegate* flameUpdateDelegatePtr)
{
    void (*OnInfoDisplay)(int) = [](int layerId)
    {
        //画面に表示する情報
        if(infoIndex != 0)
        {
            HgWSetFont(layerId, HG_HB, 15);
            HgWSetColor(layerId, vec3(0.5).ToHGColor());
            HgWSetFillColor(layerId, vec3(0.5).ToHGColor());
            HgWText(layerId, 0, WINDOW_SIZE - 15, "FPS : %5.1f FPSLimit : %s", fpsCounterPtr->GetFPS(), isFPSLimitEnabled ? "ON" : "OFF");
            HgWText(layerId, 0, WINDOW_SIZE - 30, "TotalTime : %3.1f ShaderSpeed : %3.2f", totalTime, shaderDisplayerPtr->GetShaderSpeed());
        }
        if(infoIndex == 1)
        {
            HgWText(layerId, 0, WINDOW_SIZE - 45, "SleepTime : %1.3f DeltaTime : %1.3f", sleepTime, deltaTime);
            HgWText(layerId, 0, WINDOW_SIZE - 60, "Window : %dx%d PixCount : %dx%d", WINDOW_SIZE, WINDOW_SIZE, shaderDisplayerPtr->GetPixCount(), shaderDisplayerPtr->GetPixCount());
            HgWText(layerId, 0, WINDOW_SIZE - 75, "InputKeys : %s", std::bitset<10>(inputManagerPtr->GetAnyKey()).to_string().c_str());
        }
        if(infoIndex == 2)
        {
            //上から、CalcColorsTime、RenderColorsTime、DeltaTime、SleepTimeのグラフを表示
            HgWSetFont(layerId, HG_HB, 10);
            HgWText(layerId, 0, WINDOW_SIZE - 43, "CC");
            HgWText(layerId, 0, WINDOW_SIZE - 51, "RC");
            HgWText(layerId, 0, WINDOW_SIZE - 59, "DT");
            HgWText(layerId, 0, WINDOW_SIZE - 67, "ST");
            HgWBoxFill(layerId, 15, WINDOW_SIZE - 43 + 6, shaderDisplayerPtr->GetCalcColorsDeltaTime() * 1000, 2.5f, false);
            HgWBoxFill(layerId, 15, WINDOW_SIZE - 51 + 6, shaderDisplayerPtr->GetRenderColorsDeltaTime() * 1000, 2.5f, false);
            HgWBoxFill(layerId, 15, WINDOW_SIZE - 59 + 6, deltaTime * 1000, 2.5f, false);
            HgWBoxFill(layerId, 15, WINDOW_SIZE - 67 + 6, sleepTime * 1000, 2.5f, false);
            //グラフの縦線を描画
            for (int i = 0; i <= 10; i++) HgWBoxFill(layerId, 15 + i * 10, WINDOW_SIZE - 65, 1.5f, 32, false);
        }
        if(infoIndex == 3)
        {
            HgWText(layerId, 0, WINDOW_SIZE - 45, "Connect : %d Port : %d", tcpManagerPtr->GetConnectCount(), tcpManagerPtr->GetPort());
            for (int i = 0; i < zsds.size(); i++) HgWText(layerId, 0, WINDOW_SIZE - 60 - (15 * i), "Name : %.25s", zsds[i]._device.name);
        }
    };
    
    DoubleLayerDisplayer* result = new DoubleLayerDisplayer(OnInfoDisplay);
    result->Subscribe(*flameUpdateDelegatePtr);
    return result;
}