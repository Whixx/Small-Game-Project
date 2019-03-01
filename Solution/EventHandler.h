#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <string>
#include <queue>

using namespace std;

enum Event
{
	EVENT_NULL,
	EVENT_PLAYER_WIN,
	EVENT_PLAYER_LOSE
	EVENT_PLAYER_DROPCOIN,
	EVENT_PLAYER_TOSSCOIN,
};

class EventHandler
{
public:
	EventHandler(const EventHandler& other) = delete;
	EventHandler& operator=(const EventHandler& other) = delete;

	static EventHandler& GetInstance();

	bool IsEmpty();
	Event GetEvent();
	void AddEvent(Event e);

private:
	EventHandler() {}
	//~EventHandler();

	std::queue<Event> events;

};

#endif