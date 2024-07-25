#include "event.hpp"

Event::Event(EventType type)
{
	this->type = type;
}

UseEquippedEvent::UseEquippedEvent(int cursor_x, int cursor_y) : Event(EVENT_USE_EQUIPPED)
{
	this->cursor_x = cursor_x;
	this->cursor_y = cursor_y;
}

CursorMovedEvent::CursorMovedEvent(int cursor_x, int cursor_y) : Event(EVENT_CURSOR_MOVED)
{
	this->cursor_x = cursor_x;
	this->cursor_y = cursor_y;
}

CircleSpawnedEvent::CircleSpawnedEvent(const Circle& circle) : Event(EVENT_SPAWN_OBJECT)
{
	this->circle = Circle(circle);
}

CollisionEvent::CollisionEvent(float size_difference, float velocity) : Event(EVENT_COLLISION)
{
	this->size_difference = size_difference;
	this->velocity = velocity;
}

ImplosionEvent::ImplosionEvent(int origin_x, int origin_y) : Event(EVENT_IMPLOSION)
{
	this->origin_x = origin_x;
	this->origin_y = origin_y;
}

ExplosionEvent::ExplosionEvent(int origin_x, int origin_y) : Event(EVENT_EXPLOSION)
{
	this->origin_x = origin_x;
	this->origin_y = origin_y;
}
