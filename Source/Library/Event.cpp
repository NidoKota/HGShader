#include "Event.hpp"

void Delegate::Invoke()
{
    for(int i = 0; i < funcs.size(); i++) funcs[i]();
}

EventHandler::EventHandler()
{
    isActive = true;
}

void EventHandler::Subscribe(Delegate& flameUpdateDelegate)
{
    Start();
    flameUpdateDelegate.funcs.push_back(flameUpdateFunc);
}

void EventHandler::SetActive(bool enabled)
{
    if(enabled) Start();
    isActive = enabled;
}

bool EventHandler::GetActive()
{
    return isActive;
}