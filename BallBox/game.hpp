#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <SDL.h>
#include "event_handler.hpp"
#include "event_queue.hpp"
#include "memory"
#include "input_handler.hpp"
#include "renderer.hpp"
#include "world.hpp"
#include "inventory.hpp"
#include "sound_engine.hpp"

struct GameSettings {
	int target_fps = 60;
};

class Game : public EventHandler {
public:
	Game(SDL_Renderer* sdl_renderer);
	void loop();
	void HandleEvent(const Event* event);
private:
	std::shared_ptr<EventQueue> event_queue;
	std::unique_ptr<InputHandler> input_handler;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<World> world;
	std::unique_ptr<Inventory> inventory;
	std::unique_ptr<SoundEngine> sound_engine;

	bool quit = false;
	bool paused = false;
	long long pausedAt = 0;
	long long lastFrame = 0;
	float time_between_frames;
	long long get_time();
	void save();
	void load();
	Game();
};

#endif