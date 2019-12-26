#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "MouseEventListener.h"

class EventListener
{
public:
	EventListener() = delete;
	EventListener(const EventListener&) = delete; //Copy constructor
	EventListener(EventListener&&) = delete;//move constructor
	EventListener operator=(const EventListener&) = delete;
	EventListener operator=(EventListener&&) = delete;
	~EventListener();

	static void Update();
};

#endif //!EVENTLISTENER_H