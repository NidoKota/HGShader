#pragma once

#include "../include/BaseInclude.hpp"

extern bool isFPSLimitEnabled;
extern bool isPlaying;
extern double deltaTime;
extern double sleepTime;
extern double totalTime;
extern int infoIndex;

using OnStart = void (*)();
using OnFlameUpdate = void (*)();

void MainLoop(OnStart onStart, OnFlameUpdate onFlameUpdate);