#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include "Defines.hpp"

//複数のstd::function<void(void)を保有するだけ
struct Delegate
{
    public :
    std::vector<std::function<void(void)>> funcs;

    //登録された全ての関数を実行する
    void Invoke();
};

struct EventHandler
{
    public :
    EventHandler();
    //Start関数を実行し、FlameUpdateDelegateを購読する
    virtual void Subscribe(Delegate& flameUpdateDelegate);
    //起動状態を設定する falseでFlameUpdate関数が実行されなくなる
    virtual void SetActive(bool enabled);
    //起動状態を取得
    virtual bool GetActive();

    private :
    bool isActive;
    std::function<void(void)> flameUpdateFunc = ([&](void) { if(isActive) FlameUpdate(); });
    //↑ std::bindより早いらしいが、結局std::functionに変換したので変わらなさそう
    
    virtual void Start(){ }
    virtual void FlameUpdate(){ }
};