#ifndef __EVENT_QUEUE_HPP__
#define __EVENT_QUEUE_HPP__

#include "event.hpp"
#include <unordered_map>
#include <queue>
#include "event_handler.hpp"

class EventQueue {
public:
	void subscribe(EventType type, EventHandler* handler);
	template<typename T>
	void publish(const T& event) {
		T* ptr = new T(event);
		this->queue.push(ptr);
	}
	void process();
private:
	std::unordered_map<EventType, std::vector<EventHandler*>> subscribers;
	std::queue<const Event*> queue;
};

#endif