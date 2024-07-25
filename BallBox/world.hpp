#ifndef __WORLD_HPP
#define __WORLD_HPP

#include "circle.hpp"
#include <vector>
#include <unordered_map>
#include <memory>
#include "event_queue.hpp"

struct WorldSettings {
	float gravity = 1000;
	float wall_damping_factor = 0.15f;
	int width = 1024;
	int height = 768;
	bool enable_gravity = false;
	bool enable_collisions = true;
};

class World : public EventHandler {
public:
	World(const WorldSettings& worldSettings, std::shared_ptr<EventQueue> event_queue);
	void add_object(const Circle& object);
	void clear_objects();
	void apply_physics(float secondsElapsed);
	void HandleEvent(const Event* event);
	const std::vector<Circle>& get_objects() const;
private:
	std::vector<Circle> objects;
	float gravity = 1000;
	float wall_damping_factor = 0.15f;
	int width = 1024;
	int height = 768;
	bool enable_gravity = false;
	bool enable_collisions = true;
	int current_id = 0;
	std::unordered_map<uint32_t, bool>* collisions = nullptr;
	std::shared_ptr<EventQueue> event_queue;

	void apply_gravity(Circle& object, float secondsElapsed);
	void apply_wall_collisions(Circle& object);
	void apply_object_collisions(Circle& object, std::vector<Circle>& objects);
	void apply_velocity(Circle& object, float secondsElapsed, std::vector<Circle>& objects);
	void apply_explosion(float x, float y, float factor);

	bool are_colliding(const Circle& object1, const Circle& object2) const;
	World();
};

#endif