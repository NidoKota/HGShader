#include "ShaderDisplayer.hpp"

void ShaderDisplayer::Start()
{
    if(!once)
    {
        layers = HgWAddDoubleLayer(0);
        once = true;
    }
    shaderTimeFitter = 0;
}

void ShaderDisplayer::FlameUpdate()
{
    using namespace std::chrono;
    
    hiddenLayerID = HgLSwitch(&layers);
    HgLCopy(layers.hidden, layers.display);

    shaderTime = *t * (double)GetShaderSpeed() - shaderTimeFitter;

    //Shaderの計算をする
    pixUpdateFuture = std::async(std::launch::async, [&]()
    {
        system_clock::time_point sT = system_clock::now();
        for (int x = 0; x < GetPixCount(); x++)
        {
            for (int y = 0; y < GetPixCount(); y++) PixUpdate(x, y);
        }
        calcColorsDeltaTime = clamp(duration_cast<milliseconds>(system_clock::now() - sT).count() / 1000.f, 0.f, 1.f);
    });

    if(renderCount >= 2)
    {
        //ピクセルをレンダリングする
        pixRenderFuture = std::async(std::launch::async, [&]()
        {
            system_clock::time_point sT = system_clock::now();
            for (int x = 0; x < GetPixCount(); x++)
            {
                for (int y = 0; y < GetPixCount(); y++) PixRender(x, y);
            }
            renderColorsDeltaTime = clamp(duration_cast<milliseconds>(system_clock::now() - sT).count() / 1000.f, 0.f, 1.f);
        });

        pixRenderFuture.wait();
    }

    pixUpdateFuture.wait();
    
    renderCount = renderCount >= 3 ? renderCount : renderCount + 1;
    calcColorsIndex = calcColorsIndex >= 2 ? 0 : calcColorsIndex + 1;
    renderColorsIndex = renderColorsIndex >= 2 ? 0 : renderColorsIndex + 1;
    bufferColorsIndex = bufferColorsIndex >= 2 ? 0 : bufferColorsIndex + 1;
}

void ShaderDisplayer::PixUpdate(int x, int y)
{
    //UV座標の計算
    uv.SetAll((double)x / GetPixCount(), (double)y / GetPixCount());
    colors[calcColorsIndex][x][y] = shaderFuncs[shaderFuncsIndex](uv, shaderTime, zsds);
}

void ShaderDisplayer::PixRender(int x, int y)
{
    if(renderCount > 2)
    {
        //前回と比べてピクセルの色が変わったかどうか
        if(colors[renderColorsIndex][x][y] != colors[bufferColorsIndex][x][y]) shouldColorUpdate = true;
    }
    
    if(renderCount <= 2 || shouldColorUpdate)
    {
        //色を設定してレンダリングする
        HgWSetFillColor(hiddenLayerID, colors[renderColorsIndex][x][y].ToHGColor());
        HgWBoxFill(hiddenLayerID, (double)x * pixSize, (double)y * pixSize, pixSize, pixSize, 0);
        shouldColorUpdate = false;
    }
}

void ShaderDisplayer::IncreasePixCount()
{
    SetPixCount(FindDivisibleNum(WINDOW_SIZE, GetPixCount() + 1, true));
}

void ShaderDisplayer::DecreasePixCount()
{
    SetPixCount(FindDivisibleNum(WINDOW_SIZE, GetPixCount() - 1, false));
}

int ShaderDisplayer::GetPixCount()
{
    return pixCount;
}

float ShaderDisplayer::GetShaderSpeed()
{
    return shaderSpeed;
}

void ShaderDisplayer::SetShaderSpeed(float speed)
{
    shaderTimeFitter = (*t * speed) - shaderTime;
    shaderSpeed = speed;
}

void ShaderDisplayer::SetPixCount(int pixCount)
{
    if(renderCount != 0)
    {
        pixCount = clamp(pixCount, MIN_PIX_COUNT, WINDOW_SIZE);

        //TODO:確保済みのメモリは削除させないようにする
        //前に使ってたメモリを消す
        if(&colors[0][0] != nullptr)
        {
            for (int i = 0; i < GetPixCount(); i++) 
            {
                delete[] colors[0][i];
                delete[] colors[1][i];
                delete[] colors[2][i];
            }
            delete[] colors[0];
            delete[] colors[1];
            delete[] colors[2];
        }
        
        //メモリを確保する
        colors[0] = new vec3*[pixCount];
        for (int i = 0; i < pixCount; i++) colors[0][i] = new vec3[pixCount];
        colors[1] = new vec3*[pixCount];
        for (int i = 0; i < pixCount; i++) colors[1][i] = new vec3[pixCount];
        colors[2] = new vec3*[pixCount];
        for (int i = 0; i < pixCount; i++) colors[2][i] = new vec3[pixCount];
        
        this->pixCount = pixCount;
        pixSize = (double)WINDOW_SIZE / (double)pixCount;
        renderCount = 0;
    }
}

int ShaderDisplayer::FindDivisibleNum(int value, int start, bool isNextNum)
{
    int i = start;
    for(; i < value; i += isNextNum ? 1 : -1) if (value % i == 0) break;
    return i;
}

int ShaderDisplayer::GetShaderFuncsSize()
{
    return shaderFuncsSize;
}

int ShaderDisplayer::GetShaderFuncsIndex()
{
    return shaderFuncsIndex;
}

void ShaderDisplayer::SetShaderFuncsIndex(int index)
{
    if(renderCount != 0)
    {
        index = clamp(index, 0, shaderFuncsSize);
        shaderFuncsIndex = index;

        renderCount = 0;
    }
}

double ShaderDisplayer::GetCalcColorsDeltaTime()
{
    return calcColorsDeltaTime;
}

double ShaderDisplayer::GetRenderColorsDeltaTime()
{
    return renderColorsDeltaTime;
}