#pragma once

#include "../include/ComponentInclude.hpp"

//ダブルレイヤーを用いた描画を行う
struct DoubleLayerDisplayer : public EventHandler
{
    using Func = void (*)(int);

    private : 
    doubleLayer layers;
    Func draw;
    bool once;
    int hiddenLayerID;

    protected : 
    void Start() override;
    void FlameUpdate() override;

    public : 
    //描画する処理がある関数を登録する(引数のintはlayerID)
    DoubleLayerDisplayer(Func draw);
    //起動状態を設定する
    void SetActive(bool enabled) override;
    //使用しているダブルレイヤーを取得
    doubleLayer GetDoubleLayer();
};