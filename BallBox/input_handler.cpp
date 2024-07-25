#include <SDL.h>
#include "input_handler.hpp"


InputHandler::InputHandler(std::shared_ptr<EventQueue> event_queue)
{
	this->event_queue = event_queue;
}

void InputHandler::handle_input()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			this->event_queue->publish(Event(EVENT_EXIT_GAME));
		}
		else if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.scancode == SDL_SCANCODE_C) {
				this->event_queue->publish(Event(EVENT_CLEAR));
			}
			else if (e.key.keysym.scancode == SDL_SCANCODE_G) {
				this->event_queue->publish(Event(EVENT_TOGGLE_GRAVITY));
			}
			else if (e.key.keysym.scancode == SDL_SCANCODE_C) {
				this->event_queue->publish(Event(EVENT_TOGGLE_COLLISIONS));
			}
			else if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				this->event_queue->publish(Event(EVENT_EXIT_GAME));
			}
			else if (e.key.keysym.scancode == SDL_SCANCODE_P) {
				this->event_queue->publish(Event(EVENT_PAUSE_GAME));
			}
			else if (e.key.keysym.scancode == SDL_SCANCODE_S) {
				this->event_queue->publish(Event(EVENT_BEGIN_SAVE));
			}
			else if (e.key.keysym.scancode == SDL_SCANCODE_L) {
				this->event_queue->publish(Event(EVENT_RELOAD));
			}
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				this->event_queue->publish(UseEquippedEvent(x, y));
			}
			else if (e.button.button == SDL_BUTTON_RIGHT) {
				this->event_queue->publish(Event(EVENT_EQUIP_NEXT));
			}
		}
		else if (e.type == SDL_MOUSEMOTION) {
			this->event_queue->publish(CursorMovedEvent(e.motion.x, e.motion.y));
		}
	}
}
