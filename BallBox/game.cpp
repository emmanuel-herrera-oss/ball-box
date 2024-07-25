#include "game.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <fstream>

Game::Game(SDL_Renderer* sdl_renderer)
{
	this->time_between_frames = (1.0f / GameSettings().target_fps) * 1000000.0f;
	this->event_queue = std::make_shared<EventQueue>();
	this->input_handler = std::make_unique<InputHandler>(this->event_queue);
	this->renderer = std::make_unique<Renderer>(sdl_renderer, RenderSettings());
	this->world = std::make_unique<World>(WorldSettings(), this->event_queue);
	this->inventory = std::make_unique<Inventory>(this->event_queue);
	this->sound_engine = std::make_unique<SoundEngine>(this->event_queue);

	this->event_queue->subscribe(EVENT_EXIT_GAME, this);
	this->event_queue->subscribe(EVENT_PAUSE_GAME, this);
	this->event_queue->subscribe(EVENT_BEGIN_SAVE, this);
	this->event_queue->subscribe(EVENT_RELOAD, this);

	this->load();
}

void Game::loop()
{
	while (true) {
		this->input_handler->handle_input();
		if (this->quit) return;

		this->event_queue->process(); 

		if (this->paused) continue;

		auto now = get_time();
		if (this->lastFrame == 0) {
			this->lastFrame = now;
			continue;
		}
		long long timeElapsed = now - this->lastFrame;

		this->renderer->begin();
		this->world->apply_physics(timeElapsed / 1000000.0f);
		for (const Circle& object : this->world->get_objects()) {
			this->renderer->draw(object);
		}
		this->renderer->draw(this->inventory->get_cursor());
		this->renderer->end();
		long long frameFinishTime = get_time();

		long long frame_time = frameFinishTime - now;
		if (frame_time < this->time_between_frames) {
			SDL_Delay((this->time_between_frames - frame_time) / 1000);
		}
		this->lastFrame = now;
	}
}

long long Game::get_time() {
	std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	);
	return ms.count();
}

void Game::HandleEvent(const Event* event) {
	if (event->type == EVENT_EXIT_GAME) {
		this->quit = true;
	}
	else if (event->type == EVENT_PAUSE_GAME) {
		if (!this->paused) {
			this->pausedAt = this->get_time();
			this->paused = true;
		}
		else {
			this->lastFrame = 0;
			this->paused = false;
		}
	}
	else if (event->type == EVENT_BEGIN_SAVE) {
		this->save();
	}
	else if (event->type == EVENT_RELOAD) {
		this->load();
	}
}

void Game::save()
{
	std::ofstream file;
	file.open("save.txt", std::ios::out | std::ios::trunc);
	for (const auto& object : this->world->get_objects()) {
		file << object.id << std::endl;
		file << object.radius << std::endl;
		file << object.color.r << ',' << object.color.g << ',' << object.color.b << std::endl;
		file << object.position.get_x() << ',' << object.position.get_y() << std::endl;
		file << object.velocity.get_x() << ',' << object.velocity.get_y() << std::endl;
		file << std::endl;
	}
	file.close();
}

void Game::load()
{
	this->world->clear_objects();
	std::ifstream file;
	file.open("save.txt", std::ios::in);
	if (!file.is_open()) {
		return;
	}
	std::string line;
	int idx = 0;
	Circle b;
	while (std::getline(file, line)) {
		if (line.length() == 0) continue;
		if (idx == 0) {
			b.id = std::stoi(line);
		}
		else if (idx == 1) {
			b.radius = std::stof(line);
		}
		else if (idx == 2) {
			sscanf_s(line.c_str(), "%u,%u,%u", &b.color.r, &b.color.g, &b.color.b);
		}
		else if (idx == 3) {
			float x, y;
			sscanf_s(line.c_str(), "%f,%f", &x, &y);
			b.position = Vec2d(x, y);
		}
		else if (idx == 4) {
			float x, y;
			sscanf_s(line.c_str(), "%f,%f", &x, &y);
			b.velocity = Vec2d(x, y);
		}
		if (idx == 4) {
			idx = 0;
			this->world->add_object(b);
		}
		else {
			idx++;
		}
	}
}