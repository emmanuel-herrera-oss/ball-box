#ifndef __RENDERER_HPP
#define __RENDERER_HPP

#include <SDL.h>
#include "circle.hpp"

struct RenderSettings {
	int segments_per_object = 64;
};

class Renderer {
public:
	Renderer(SDL_Renderer* sdl_renderer, const RenderSettings& render_settings);
	void begin() const;
	void end() const;
	void draw(const Circle& object) const;
private:
	Renderer();
	SDL_Renderer* sdl_renderer;
	int segments_per_object;
};

#endif