#ifndef __INVENTORY_HPP__
#define __INVENTORY_HPP__

#include "circle.hpp"
#include "event_handler.hpp"
#include "memory"
#include "event_queue.hpp"

class Inventory : public EventHandler {
public:
	Inventory(std::shared_ptr<EventQueue> event_queue);
	void HandleEvent(const Event* event);
	const Circle& get_cursor();

private:
	int item_index = -1;
	void equip_next();
	void use_equipped();
	Circle cursor;
	std::shared_ptr<EventQueue> event_queue;
	Inventory();
};

#endif