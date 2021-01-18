#include "InputManager.hpp"

void InputManager::Start()
{
    HgSetEventMask(HG_KEY_EVENT_MASK | HG_KEY_UP | HG_KEY_DOWN);
}

void InputManager::FlameUpdate()
{
	inputDown = 0;
    hgevent *event = HgEventNonBlocking();
    if(event != nullptr && observeKeyValues.count(event->ch) > 0)
	{
		if(event->type == HG_KEY_DOWN) input |= (1 << observeKeyValues[event->ch]);
	    if(event->type == HG_KEY_UP) input &= ~(1 << observeKeyValues[event->ch]);
	}
	
	inputDown = ~prevInput & input;
	inputUp = prevInput & ~input;
	prevInput = input;
}

bool InputManager::GetKey(unsigned int key)
{
	return input & (1 << observeKeyValues[key]);
}

bool InputManager::GetKeyDown(unsigned int key)
{
	return inputDown & (1 << observeKeyValues[key]);
}

bool InputManager::GetKeyUp(unsigned int key)
{
	return inputUp & (1 << observeKeyValues[key]);
}

unsigned int InputManager::GetAnyKey()
{
	return input;
}