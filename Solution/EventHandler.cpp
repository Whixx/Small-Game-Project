#include "EventHandler.h"

EventHandler & EventHandler::GetInstance()
{
	static EventHandler instance;
	return instance;
}

bool EventHandler::IsEmpty()
{
	return this->events.empty();
}

Event EventHandler::GetEvent()
{
	if (this->events.empty())
	{
		return EVENT_NULL;
	}
	Event currentEvent = this->events.front();
	this->events.pop();
	return currentEvent;
}

void EventHandler::AddEvent(Event e)
{
	this->events.push(e);
}
