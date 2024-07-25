#ifndef __INPUT_HANDLER_HPP__
#define __INPUT_HANDLER_HPP__

#include "event_queue.hpp"
#include "memory"

class InputHandler {
public:
	InputHandler(std::shared_ptr<EventQueue> event_queue);
	void handle_input();
private:
	InputHandler();
	std::shared_ptr<EventQueue> event_queue;
};

#endif