#pragma once

#include "ShaderFuncs.hpp"
#include "../include/ComponentInclude.hpp"

#include "MainLoop.hpp"

FPSCounter* GetFPSCounterPtr(Delegate* flameUpdateDelegatePtr);
TCPManager* GetTCPManagerPtr(Delegate* flameUpdateDelegatePtr);
InputManager* GetInputManagerPtr(Delegate* flameUpdateDelegatePtr);
ShaderDisplayer* GetShaderDisplayerPtr(Delegate* flameUpdateDelegatePtr);
DoubleLayerDisplayer* GetInfoDisplayPtr(Delegate* flameUpdateDelegatePtr);

extern FPSCounter* fpsCounterPtr;
extern TCPManager* tcpManagerPtr;
extern InputManager* inputManagerPtr;
extern ShaderDisplayer* shaderDisplayerPtr;
extern DoubleLayerDisplayer* infoDisplayerPtr;