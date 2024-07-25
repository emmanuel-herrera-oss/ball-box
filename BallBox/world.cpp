#include "world.hpp"
#include <stdexcept>

World::World(const WorldSettings& world_settings, std::shared_ptr<EventQueue> event_queue)
{
	if (world_settings.gravity < 0) {
		throw std::invalid_argument("gravity must be greater than or equal to zero.");
	}
	if (world_settings.width <= 0 || world_settings.height <= 0) {
		throw std::invalid_argument("width and height must be greater than zero.");
	}
	this->gravity = world_settings.gravity;
	this->width = world_settings.width;
	this->height = world_settings.height;
	this->wall_damping_factor = world_settings.wall_damping_factor;
	this->enable_collisions = world_settings.enable_collisions;
	this->enable_gravity = world_settings.enable_gravity;
	this->collisions = new std::unordered_map<uint32_t, bool>();
	event_queue->subscribe(EVENT_CLEAR, this);
	event_queue->subscribe(EVENT_TOGGLE_GRAVITY, this);
	event_queue->subscribe(EVENT_TOGGLE_COLLISIONS, this);
	event_queue->subscribe(EVENT_SPAWN_OBJECT, this);
	event_queue->subscribe(EVENT_EXPLOSION, this);
	event_queue->subscribe(EVENT_IMPLOSION, this);
	this->event_queue = event_queue;
}

void World::add_object(const Circle& object)
{
	Circle copy = object;
	copy.id = this->current_id++;
	this->objects.push_back(copy);
}

void World::clear_objects()
{
	this->objects.clear();
}

void World::apply_physics(float secondsElapsed)
{
	for (Circle& object : this->objects) {
		this->apply_gravity(object, secondsElapsed);
		this->apply_wall_collisions(object);
		this->apply_object_collisions(object, this->objects);
	}
	for (Circle& object : this->objects) {
		this->apply_velocity(object, secondsElapsed, this->objects);
	}
}

void World::HandleEvent(const Event* event)
{
	if (event->type == EVENT_CLEAR) {
		this->clear_objects();
	}
	else if (event->type == EVENT_TOGGLE_GRAVITY) {
		this->enable_gravity = !this->enable_gravity;
	}
	else if (event->type == EVENT_TOGGLE_COLLISIONS) {
		this->enable_collisions = !this->enable_collisions;
	}
	else if (event->type == EVENT_SPAWN_OBJECT) {
		auto spawn_event = dynamic_cast<const CircleSpawnedEvent*>(event);
		this->add_object(spawn_event->circle);
	}
	else if (event->type == EVENT_EXPLOSION) {
		auto explosion_event = dynamic_cast<const ExplosionEvent*>(event);
		this->apply_explosion(explosion_event->origin_x, explosion_event->origin_y, -100000.0f);
	}
	else if (event->type == EVENT_IMPLOSION) {
		auto implosion_event = dynamic_cast<const ImplosionEvent*>(event);
		this->apply_explosion(implosion_event->origin_x, implosion_event->origin_y, 10000.0f);
	}
}

const std::vector<Circle>& World::get_objects() const
{
	return this->objects;
}

void World::apply_gravity(Circle& object, float secondsElapsed)
{
	if (this->enable_gravity == false) return;
	if (object.position.get_y() + object.radius >= this->height) return;
	object.velocity = object.velocity.add(Vec2d(0, this->gravity * secondsElapsed));
}

void World::apply_wall_collisions(Circle& object)
{
	bool any = false;
	if (object.position.get_y() + object.radius >= this->height && object.velocity.get_y() > 0) {
		if (object.velocity.get_y() >= 50.0f) {
			any = true;
		}
		object.velocity = Vec2d(object.velocity.get_x(), (1.0f - this->wall_damping_factor) * -1.0f * object.velocity.get_y());
	}
	if (object.position.get_y() - object.radius <= 0 && object.velocity.get_y() < 0) {
		object.velocity = Vec2d(object.velocity.get_x(), (1.0f - this->wall_damping_factor) * -1.0f * object.velocity.get_y());
		any = true;
	}
	if (object.position.get_x() + object.radius >= this->width && object.velocity.get_x() > 0) {
		object.velocity = Vec2d((1.0f - this->wall_damping_factor) * -object.velocity.get_x(), object.velocity.get_y());
		any = true;
	}
	if (object.position.get_x() - object.radius <= 0 && object.velocity.get_x() < 0) {
		object.velocity = Vec2d((1.0f - this->wall_damping_factor) * -object.velocity.get_x(), object.velocity.get_y());
		any = true;
	}
	if (any) {
		this->event_queue->publish(CollisionEvent(object.radius, object.velocity.magnitude()));
	}
}

void World::apply_object_collisions(Circle& object, std::vector<Circle>& objects)
{
	if (!this->enable_collisions) return;
	for (Circle& otherObject : objects) {
		if (object.id == otherObject.id) continue;
		if (!this->are_colliding(object, otherObject)) continue;

		Vec2d normal = otherObject.position.subtract(object.position).normalize();
		Vec2d tangent = normal.get_orthogonal();
		float v1n = normal.dot(object.velocity);
		float v1t = tangent.dot(object.velocity);
		float v2n = normal.dot(otherObject.velocity);
		float v2t = tangent.dot(otherObject.velocity);

		// assume mass is proportional to radius
		float m1 = object.radius;
		float m2 = otherObject.radius;

		float v1n2 = (v1n * (m1 - m2) + 2 * m2 * v2n) / (m1 + m2);
		float v2n2 = (v2n * (m2 - m1) + 2 * m1 * v1n) / (m1 + m2);

		Vec2d v1nPrime = normal.scalar_multiply(v1n2);
		Vec2d v2nPrime = normal.scalar_multiply(v2n2);
		Vec2d v1tPrime = tangent.scalar_multiply(v1t);
		Vec2d v2tPrime = tangent.scalar_multiply(v2t);

		object.velocity = v1nPrime.add(v1tPrime);
		otherObject.velocity = v2nPrime.add(v2tPrime);
	}
}

void World::apply_velocity(Circle& object, float secondsElapsed, std::vector<Circle>& objects)
{
	object.position = object.position.add(object.velocity.scalar_multiply(secondsElapsed));
}

void World::apply_explosion(float x, float y, float factor)
{
	auto origin = Vec2d(x, y);
	for (Circle& object : this->objects) {
		auto direction = origin.subtract(object.position);
		float distance = direction.magnitude();
		float appliedForce = 1 / distance;
		object.velocity = object.velocity.add(direction.normalize().scalar_multiply(factor * appliedForce));
	}
}

bool World::are_colliding(const Circle& object1, const Circle& object2) const
{
	float distance = object2.position.subtract(object1.position).magnitude();
	float totalRadius = object1.radius + object2.radius;
	float threshold = 0.01f;

	bool colliding = (distance - totalRadius <= threshold);
	
	auto collisionId = (std::min(object1.id, object2.id) & 0x0000FFFF) | ((std::max(object1.id, object2.id) << 16) & 0xFFFF0000); //TODO: Allow no more than 2^16 objects...
	bool midCollision = (*this->collisions)[collisionId];
	if (colliding && !midCollision) {
		(*this->collisions)[collisionId] = true;
		this->event_queue->publish(CollisionEvent(abs(object1.radius - object2.radius), object1.velocity.subtract(object2.velocity).magnitude()));
		return true;
	}
	if (!colliding && midCollision) {
		(*this->collisions)[collisionId] = false;
	}
	return false;
}
