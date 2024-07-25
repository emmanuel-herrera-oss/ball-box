#ifndef __EVENT_HANDLER_HPP
#define __EVENT_HANDLER_HPP

#include "event.hpp"

class EventHandler {
public:
	virtual void HandleEvent(const Event* event) = 0;
};

#endif