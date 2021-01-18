#include <iostream>
#include <math.h>
#include <map>
#include "../Library/Defines.hpp"
#include "../Library/ShaderUtility.hpp"
#include "../Library/Event.hpp"

//簡単にキー入力を取得できるようにする
struct InputManager : public EventHandler
{
private :
    std::map<unsigned int, int> observeKeyValues;
    int currentObserveKeyValuesIndex;

    unsigned int input;
    unsigned int prevInput;

    unsigned int inputDown;
    unsigned int inputUp;

	void Start() override;
	void FlameUpdate() override;

    template<typename First, typename... Rest>
    void RoundUp(const First& first, const Rest&... rest)
    {
        observeKeyValues.emplace(first, currentObserveKeyValuesIndex);
        currentObserveKeyValuesIndex++;
        RoundUp(rest...);
    }

    void RoundUp(){}

public : 
    //監視したいキーを登録する
    template<typename First, typename... Rest>
    InputManager(const First& first, const Rest&... rest)
    {
        RoundUp(first, rest...);
    }
    
    //キーが入力されているかを取得
    bool GetKey(unsigned int key);
    //現在のフレームでキーが押されたかを取得
    bool GetKeyDown(unsigned int key);
    //現在のフレームでキーが離されたかを取得
    bool GetKeyUp(unsigned int key);
    //どのキーが押されているかを取得
    //返り値はビットフラグになっていて、インスタンス時に指定したキーの順番で右からビットが立つ
    unsigned int GetAnyKey();
};