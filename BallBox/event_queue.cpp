#include "event_queue.hpp"

void EventQueue::subscribe(EventType type, EventHandler* handler)
{
	auto& subscribers = this->subscribers[type];
	for (const auto& element : subscribers) {
		if (element == handler) return; // Prevent duplicates
	}
	subscribers.push_back(handler);
}

void EventQueue::process()
{
	while (!this->queue.empty()) {
		const Event* evt = this->queue.front();
		this->queue.pop();
		const auto& subscribers = this->subscribers[evt->type];
		for (const auto& element : subscribers) {
			element->HandleEvent(evt);
		}
		delete evt;
	}
}
