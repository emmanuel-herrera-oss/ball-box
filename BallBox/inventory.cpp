#include "inventory.hpp"
#include <stdexcept>

Inventory::Inventory(std::shared_ptr<EventQueue> event_queue)
{
	equip_next();
	event_queue->subscribe(EVENT_CURSOR_MOVED, this);
	event_queue->subscribe(EVENT_EQUIP_NEXT, this);
	event_queue->subscribe(EVENT_USE_EQUIPPED, this);
	this->event_queue = event_queue;
}

void Inventory::HandleEvent(const Event* event) {
	if (event->type == EVENT_CURSOR_MOVED) {
		auto cursorMovedEvent = dynamic_cast<const CursorMovedEvent*>(event);
		this->cursor.position = Vec2d(cursorMovedEvent->cursor_x, cursorMovedEvent->cursor_y);
	}
	else if (event->type == EVENT_EQUIP_NEXT) {
		equip_next();
	}
	else if (event->type == EVENT_USE_EQUIPPED) {
		use_equipped();
	}
}

const Circle& Inventory::get_cursor()
{
	return this->cursor;
}

void Inventory::equip_next()
{
	this->item_index = ++this->item_index > 4 ? 0 : this->item_index;
	if (item_index == 0) {
		cursor.color = { 0xFF, 0, 0 };
		cursor.radius = 10;
	}
	else if (item_index == 1) {
		cursor.color = { 0, 0xFF, 0 };
		cursor.radius = 20;
	}
	else if (item_index == 2) {
		cursor.color = { 0, 0, 0xFF };
		cursor.radius = 60;
	}
	else if (item_index == 3) {
		cursor.color = { 0, 0, 0 };
		cursor.radius = 5;
	}
	else if (item_index == 4) {
		cursor.color = { 0xFF, 0x00, 0xFF };
		cursor.radius = 5;
	}
	else {
		throw std::invalid_argument("Invalid item_index");
	}
}

void Inventory::use_equipped()
{
	if (this->item_index == 3) {
		Event evt = Event(EVENT_IMPLOSION);
		this->event_queue->publish(ImplosionEvent(this->cursor.position.get_x(), this->cursor.position.get_y()));
	}
	else if (this->item_index == 4) {
		Event evt = Event(EVENT_EXPLOSION);
		this->event_queue->publish(ExplosionEvent(this->cursor.position.get_x(), this->cursor.position.get_y()));
	}
	else {
		Circle spawnedObject = this->cursor;
		this->event_queue->publish(CircleSpawnedEvent(spawnedObject));
	}
}
