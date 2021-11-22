#pragma once

#include "../../include/SubInclude.hpp"

struct device
{
public:
    const char* name;
    int displayheight;
    const char* uuid;
    const char* os;
    const char* osversion;
    int displaywidth;
};

struct sensordata
{
public:
    vec4 quaternion;
};

//ZigSimから受け取ったデータ(一部のみ実装)
struct zigsimdata
{
public:
    device _device;
    sensordata _sensorData;
    const char* timestamp;

    //Jsonを解析してデータに変換する
    void Parse(char* data);
};
