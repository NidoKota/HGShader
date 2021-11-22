<!--

GitHub上で見る場合
https://github.com/NidoKota/HGShader/blob/main/README.md

-->

# HGShader

<img width="270" alt="" src="manualimages/0.gif"> <img width="270" alt="" src="manualimages/1.gif"> <img width="270" alt="" src="manualimages/2.gif">
<br>

<a href="http://www7a.biglobe.ne.jp/~ogihara/Hg/hg-jpn.html" target="_blank" rel="noopener noreferrer">HandyGraphic</a> で本格的なシェーディングがしたい…！<br>
自分のピクセルシェーダーをC++に移植してデバッグしたい…！<br>
HGShaderはそんなニッチで意味不明な需要を満たすプログラムです<br>

# 動作原理
毎フレーム画面上に大量の四角い図形を並べています<br>
初期状態の50x50ピクセルで2500個のHgBoxFillが表示されます<br>
FPS数(FramePerSecond)を稼ぐためにいくつかの軽量化も行っています<br>

# 紹介動画
https://user-images.githubusercontent.com/36328961/125702413-61206a6e-7483-4179-ba0f-f18752d57c3d.mp4

# サンプルの起動方法
サンプルをコンパイル後、HandyGraphicを起動させた状態で、実行ファイル(`build/app`)を実行します<br>

# サンプルのコンパイル方法
Makefileを導入した状態で`make`を実行します<br>
なお、スマホ接続時のJson解析に<a href="https://rapidjson.org/" target="_blank" rel="noopener noreferrer">RapidJson</a>を使用しているので、その導入が必要です

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

# スマホを接続する

https://user-images.githubusercontent.com/36328961/142802126-4341ac6e-0e66-4996-9616-4cb359e3e5e6.mp4

<a href="https://zig-project.com/" target="_blank" rel="noopener noreferrer">ZIGSIM</a>(無料版でも可)を使用し、スマホと接続することができます<br>
接続すると、センサー情報を使用してシェーダーを動かすことができるようになります<br>
サンプルでは`PhoneShader`のみ対応しています<br>
このシェーダーではスマホの回転情報を受け取り、オブジェクトを回転させます<br>

# 表示される情報
<img width="270" alt="" src="manualimages/3.png"><br>
- FPS : FPS数<br>
- FPSLimit : FPS数制限を設定しているかどうか<br>
- Time : 起動してからの経過時間<br>
- ShaderSpeed : シェーダーの再生速度<br>
- SleepTime : 余計なフレームを生成しないためにThreadを止めた時間<br>
- DeltaTime : 1フレーム生成するのにかかった時間<br>
- Window : 現在のWindowSize<br>
- PixCount : 現在表示されているピクセルの数<br>
- InputKeys : 入力のビットフラグ<br>
- グラフ : <br>
&nbsp;UP : シェーダーの計算時間<br>
&nbsp;RT : ピクセルのレンダリング時間<br>
&nbsp;DT : DeltaTime<br>
&nbsp;ST : SleepTime<br>
&nbsp;(目盛りは10ms)<br>

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
<a href="LICENSE" target="_blank" rel="noopener noreferrer">MIT License</a><br>
何か変更したい点などあればご連絡ください
