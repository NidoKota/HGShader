<!--

GitHub上で見る場合
https://github.com/NidoKota/HGShader/blob/main/CODEMANUAL.md

-->

# コードマニュアル
主なコードについて説明します<br>
細かい部分は説明しません<br>

# Library/Event.cpp
簡易的なイベントを実装しています<br>
EventHandlerを継承すると、overrideしたStart、FlameUpdate関数が呼び出されます<br>
また起動状態を管理するGetActive、SetActive関数が追加されます<br>
使用する際には、Delegate構造体を使ってイベントを購読する必要があります<br>

```C++
struct Sample : public EventHandler
{
    private : 
    void Start override()
    {
        std::cout << "Start" << std::endl;
    }

    void FlameUpdate override()
    {
        std::cout << "FlameUpdate" << std::endl;
    }
};
```

```C++
Sample sample;
Delegate flameUpdateDelegate;

//イベント購読と同時にStart関数実行
sample.Subscribe(flameUpdateDelegate);

for(int i = 0; i < 5; i++)
{
    //FlameUpdate関数実行
    //(SetActive関数でflaseに設定されている場合は呼び出されません)
    flameUpdateDelegate.Invoke();
}
```

# EventHandlers/ShaderDisplayer.cpp
登録したシェーダーを表示します<br>

```C++
//経過時間が入った変数のポインタと、シェーダー関数のポインタを登録
//シェーダー関数に対して、UV座標と経過時間を渡し、ピクセルの色を受け取ります
ShaderDisplayer shaderDisplayer = ShaderDisplayer(&t, Shader1, Shader2, ...etc);

//イベント購読
shaderDisplayer.Subscribe(flameUpdateDelegate);

//再度初期化処理を行う
shaderDisplayer.Start();

//起動状態を設定する
shaderDisplayer.SetActive(false);

//現在の起動状態を取得
bool active = shaderDisplayer.GetActive();
```

```C++
//破綻しないように解像度を上げる
shaderDisplayer.IncreasePixCount();

//破綻しないように解像度を下げる
shaderDisplayer.DecreasePixCount();

//シェーダーの再生速度を変更
shaderDisplayer.SetShaderSpeed(2.f);

//シェーダーを切り替える
shaderDisplayer.SetShaderFuncsIndex(1);

//ピクセルの個数を取得
int pixCount = shaderDisplayer.GetPixCount();

//登録されたシェーダーの個数を取得
int shaderFuncsSize = shaderDisplayer.GetShaderFuncsSize();

//現在実行されてるシェーダーの要素番号を取得
int shaderFuncsIndex = shaderDisplayer.GetShaderFuncsIndex();

//フレームを描画するのに、どれだけのピクセルをアップデートしたかを取得
unsigned int updatePixCount = shaderDisplayer.GetUpdatePixCount();

//現在のシェーダーの再生速度を取得
float shaderSpeed = shaderDisplayer.GetShaderSpeed();
```

# EventHandlers/InputManager.cpp
簡単にキー入力を受け取ることができます<br>
押された瞬間や、離された瞬間を取得する事もできます<br>

```C++
//監視したいキーを登録
InputManager inputManager = InputManager(HG_U_ARROW, 'a', ...etc);

//イベント購読
inputManager.Subscribe(flameUpdateDelegate);

//起動状態を設定する
inputManager.SetActive(false);

//現在の起動状態を取得
bool active = inputManager.GetActive();
```

```C++
//キーが入力されているかを取得
bool aKey = inputManager.GetKey('a');

//現在のフレームでキーが押されたかを取得
bool aKeyDown = inputManager.GetKeyDown('a');

//現在のフレームでキーが離されたかを取得
bool aKeyUp = inputManager.GetKeyUp('a');

//どのキーが押されているかを取得
//返り値はビットフラグになっていて、インスタンス時に指定したキーの順番で右からビットが立つ
unsigned int keyFlags = inputManager.GetAnyKey();
```

# EventHandlers/FPSCounter.cpp
FPS数(FramePerSecond)を計ることができます<br>

```C++
//経過時間が入った変数のポインタと、何秒ごとにFPS数を更新するかを入力
FPSCounter fpsCounter = FPSCounter(&t, 0.5f);

//イベント購読
fpsCounter.Subscribe(flameUpdateDelegate);

//再度初期化処理を行う
inputManager.Start();

//起動状態を設定する
fpsCounter.SetActive(false);

//現在の起動状態を取得
bool active = fpsCounter.GetActive();
```

```C++

//FPS数を取得
float fps = inputManager.GetFPS();
```

# EventHandlers/DoubleLayerDisplayer.cpp
簡単にダブルレイヤーを用いた描画を行うことができます

```C++
//ダブルレイヤーで描画する内容を記述したvoid(*)(int)型の関数ポインタを登録
//(引数のintはレイヤーID)
DoubleLayerDisplayer doubleLayerDisplayer = DoubleLayerDisplayer(Render);

void Render(int layerID)
{
    HGWBox(layerID, 10, 20, 5, 5);
}

//起動状態を設定する
doubleLayerDisplayer.SetActive(false);

//現在の起動状態を取得
bool active = doubleLayerDisplayer.GetActive();
```

```C++
//使用しているダブルレイヤーを取得
doubleLayer dl = GetDoubleLayer();
```
