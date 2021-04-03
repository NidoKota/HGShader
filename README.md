<!--

GitHub上で見る場合
https://github.com/NidoKota/HGShader/blob/main/README.md

-->

# HGShader

<img width="275" alt="" src="ManualImages/0.gif"> <img width="275" alt="" src="ManualImages/1.gif"> <img width="275" alt="" src="ManualImages/2.gif">
<br>

<a href="http://www7a.biglobe.ne.jp/~ogihara/Hg/hg-jpn.html" target="_blank" rel="noopener noreferrer">HandyGraphic</a> で本格的なシェーディングがしたい…！<br>
自分のピクセルシェーダーをC++に移植してデバッグしたい…！<br>
HGShaderはそんなニッチで意味不明な需要を満たすプログラムです<br>

# 動作原理
毎フレーム画面上に大量の四角い図形を並べています<br>
初期状態の50x50ピクセルで2500個のHgBoxFillが表示されます<br>
FPS数(FramePerSecond)を稼ぐためにいくつかの軽量化も行っています<br>

# サンプルの起動方法
サンプルシェーダーが動く実行ファイルがあります<br>
HandyGraphicを起動させた状態で、Sampleをダブルクリックします

# サンプルのコンパイル方法
Sourceフォルダ内で以下のコードを実行します<br>
`g++ Main.cpp ShaderFuncs.cpp EventHandlers/ShaderDisplayer.cpp EventHandlers/InputManager.cpp EventHandlers/FPSCounter.cpp EventHandlers/DoubleLayerDisplayer.cpp Library/ShaderUtility.cpp Library/Event.cpp -o Sample -lhg -std=c++11 -pthread`<br>
<br>
または、Makefileを導入した状態で`make compileRun`を実行します<br>
この場合、コンパイル後にHandyGraphicとサンプルが自動的に起動します

# 操作方法
基本の操作<br>
- 上下の方向キー : 解像度を上げ下げする<br>
- 左右の方向キー : 別のシェーダーに切り替える<br>
- スペースキー : 情報の表示/非表示を切り替える<br>

その他の操作<br>
- カンマキー( , ) : 再生速度を上げる<br>
- ドットキー( . ) : 再生速度を下げる<br>
- r キー : 時間をリセットする<br>
- f キー : FPS数制限を設定/解除する<br>
- q キー : HandyGraphicを終了する<br>

# 表示される情報
<img width="275" alt="" src="ManualImages/3.png"><br>
- FPS : FPS数<br>
- FPSLimit : FPS数制限を設定しているかどうか<br>
- Time : 起動してからの経過時間<br>
- ShaderSpeed : シェーダーの再生速度<br>
- SleepTime : 余計なフレームを生成しないためにThreadを止めた時間<br>
- DeltaTime : 1フレーム生成するのにかかった時間<br>
- Window : 現在のWindowSize<br>
- PixCount : 現在表示されているピクセルの数<br>
- InputKeys : 入力のビットフラグ<br>
- グラフ : 上から以下の情報を表示する<br>
シェーダーの計算時間、ピクセルのレンダリング時間、1フレーム生成にかかった総合時間、Threadの停止時間<br>
縦線は時間の長さを表し、1つ目の線は10msを表す<br>

# サンプルにシェーダーを追加する
`Main.cpp` 内のShaderDisplayerの引数にシェーダーを登録すると表示されるようになります<br>
登録できるピクセルシェーダーは、<br>
引数にUV座標と経過時間を取り、返り値でそのピクセルの色を返す`vec3(*)(vec3, float)`型の関数のみです<br>
なお、シェーダーはGLSLと似た形式で書くことができます<br>

# シェーダーを書く際の注意点

## 前フレームとの差分を減らす
少しでも処理を軽くするために、色を更新する領域が多いシェーダーは避けてください<br>
前フレームのピクセルと同じ色であれば、HGShaderはそのピクセルの描画をスキップし、レンダリングを早めます<br>
また、`Library/Defines.hpp` の `COLOR_CONT`を下げ、表示する色数を減らす検討もしてください

## 低解像度でも楽しめるシェーダーにする
低解像度であれば、ほとんどのシェーダーは安定して動作します<br>
様々な環境で動作させるために、低解像度でも楽しめるシェーダーを書くことが重要です

## vec3のみで頑張る
vec2やmat3、mat3x4等はありません<br>
vec3で代用したりvec3の配列で実装してください<br>
~~vec4はダメかも~~<br>

## 数字の後に f を付ける
C++では小数点を含む数字はdoubleに解釈されるのでtemplate関数でエラーが発生します<br>
数字の後の f を癖付けてください<br>

## 名称が異なる関数がある
GLSLのatan関数は、math.hのatan関数(挙動が異なる)と名前が被っているのでatan2関数として実装しています<br>
また、GLSLのnot関数は、C++のnotキーワードと名前が被っているので_not関数として実装しています<br>

# その他のコードの説明
<a href="CODEMANUAL.md" target="_blank" rel="noopener noreferrer">CODEMANUAL</a> をご覧ください<br>

# HandyGraphicの知見
<a href="HGKNOWLEDGE.md" target="_blank" rel="noopener noreferrer">HGKNOWLEDGE</a> をご覧ください<br>

# 感想
半年前の3Dの図形描画より滑らかになって楽しかった<br>
この形式で初期のウィザードリィとかDoomとか移植できそう<br>
けどもう絶対にやらない<br>
~~後輩の方に任せます~~<br>

# 権利者表示
疑似乱数に関して<br>
W.J.J. Rey, 22nd European Meeting of Statisticians and the 7th Vilnius Conference on Probability Theory and Mathematical Statistics, August 1998<br>

グラデーションノイズに関して<br>
Inigo Quilez - iq/2013<br>
<a href="https://www.shadertoy.com/view/XdXGW8" target="_blank" rel="noopener noreferrer">https://www.shadertoy.com/view/XdXGW8</a><br>

# ライセンス
<a href="LICENSE" target="_blank" rel="noopener noreferrer">MIT License</a>
