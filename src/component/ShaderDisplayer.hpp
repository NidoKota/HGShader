#pragma once

#include "../include/ComponentInclude.hpp"

//シェーダーを表示する
struct ShaderDisplayer : public EventHandler
{
    using Func = vec3 (*)(vec3, float, std::vector<zigsimdata>*);
    
private :
    std::vector<Func> shaderFuncs;
    doubleLayer layers = {};
    vec3 **colors[3];
    vec3 uv;
    std::future<void> pixUpdateFuture;
    std::future<void> pixRenderFuture;
    std::vector<zigsimdata>* zsds;
    double* t;
    double pixSize;
    double shaderTime;
    double shaderTimeFitter;
    double calcColorsDeltaTime;
    double renderColorsDeltaTime;
    float shaderSpeed;
    int hiddenLayerID;
    int shaderFuncsSize;
    int pixCount;
    int shaderFuncsIndex;
    int calcColorsIndex;
    int renderColorsIndex;
    int bufferColorsIndex;
    int renderCount;
    bool shouldColorUpdate;
    bool once;

    void FlameUpdate() override;
    int FindDivisibleNum(int value, int start, bool isNextNum);
    void SetPixCount(int pixCount);
    void PixUpdate(int x, int y);
    void PixRender(int x, int y);
    
    template<typename First, typename... Rest>
    void RoundUp(const First& first, const Rest&... rest)
    {
        shaderFuncs.push_back(first);
        RoundUp(rest...);
    }

    void RoundUp(){}

public :
    //経過時間が入った変数のポインタと、シェーダー関数のポインタを登録
    //シェーダー関数に対して、UV座標と経過時間を渡し、ピクセルの色を受けとる
    template<typename First, typename... Rest>
    ShaderDisplayer(double* t, std::vector<zigsimdata>* zsds, const First& first, const Rest&... rest)
    {
        this->t = t;
        this->zsds = zsds;
        RoundUp(first, rest...);

        shaderSpeed = 1;
        shaderFuncsSize = shaderFuncs.size();

        calcColorsIndex = 0;
        renderColorsIndex = -1;
        bufferColorsIndex = -2;

        renderCount = 1;
        SetPixCount(FindDivisibleNum(WINDOW_SIZE, 50, -1));
    }

    //再度初期化処理を行う
    void Start() override;
    //ピクセルの個数を取得
    int GetPixCount();
    //登録されたシェーダーの個数を取得
    int GetShaderFuncsSize();
    //現在実行されてるシェーダーの要素番号を取得
    int GetShaderFuncsIndex();
    //現在のシェーダーの再生速度を取得
    float GetShaderSpeed();
    //シェーダーの再生速度を変更
    void SetShaderSpeed(float timeScale);
    //破綻しないように解像度を上げる
    void IncreasePixCount();
    //破綻しないように解像度を下げる
    void DecreasePixCount();
    //シェーダーを切り替える
    void SetShaderFuncsIndex(int index);
    //Shaderの計算にかかった時間を取得
    double GetCalcColorsDeltaTime();
    //ピクセルのレンダリング時間を取得
    double GetRenderColorsDeltaTime();
};