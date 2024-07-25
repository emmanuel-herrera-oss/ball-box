#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include "circle.hpp"

enum EventType {
	EVENT_EXIT_GAME,
	EVENT_KEY_UP,
	EVENT_COLLISION,
	EVENT_CLEAR,
	EVENT_TOGGLE_GRAVITY,
	EVENT_TOGGLE_COLLISIONS,
	EVENT_USE_EQUIPPED,
	EVENT_EQUIP_NEXT,
	EVENT_CURSOR_MOVED,
	EVENT_SPAWN_OBJECT,
	EVENT_PAUSE_GAME,
	EVENT_EXPLOSION,
	EVENT_IMPLOSION,
	EVENT_BEGIN_SAVE,
	EVENT_RELOAD,
};

class Event {
public:
	Event(EventType type);
	EventType type;
	virtual ~Event() {

	}
};

// Physics Events
class CollisionEvent : public Event {
public:
	CollisionEvent(float size_difference, float velocity);
	float size_difference;
	float velocity;
};

class ExplosionEvent : public Event {
public:
	ExplosionEvent(int origin_x, int origin_y);
	int origin_x;
	int origin_y;
};

class ImplosionEvent : public Event {
public:
	ImplosionEvent(int origin_x, int origin_y);
	int origin_x;
	int origin_y;
};

// Input events 
class UseEquippedEvent : public Event {
public:
	UseEquippedEvent(int cursor_x, int cursor_y);
	int cursor_x;
	int cursor_y;
};

class CursorMovedEvent : public Event {
public:
	CursorMovedEvent(int cursor_x, int cursor_y);
	int cursor_x;
	int cursor_y;
};

class CircleSpawnedEvent : public Event {
public:
	CircleSpawnedEvent(const Circle& circle);
	Circle circle;
};

#endif