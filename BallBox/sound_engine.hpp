#ifndef __SOUND_ENGINE_HPP__
#define __SOUND_ENGINE_HPP__

#include "event_handler.hpp"
#include "event_queue.hpp"
#include "sound.hpp"
#include <memory>

class SoundEngine : public EventHandler {
public:
	SoundEngine(std::shared_ptr<EventQueue> event_queue);
	void HandleEvent(const Event* event);
	Sound sounds[16];
	bool paused = false;
private:
	SoundEngine();
	std::shared_ptr<EventQueue> event_queue;
	int sound_buffer_index = 0;
};

#endif