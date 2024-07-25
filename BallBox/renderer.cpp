#include "Renderer.hpp"
#include <stdexcept>

Renderer::Renderer(SDL_Renderer* sdl_renderer, const RenderSettings& renderSettings)
{
	if (renderSettings.segments_per_object < 4) throw std::invalid_argument("segments_per_object must be greater than or equal to four.");
	this->sdl_renderer = sdl_renderer;
	this->segments_per_object = renderSettings.segments_per_object;
}

void Renderer::begin() const
{
	SDL_RenderClear(this->sdl_renderer);
}

void Renderer::end() const
{
	SDL_RenderPresent(this->sdl_renderer);
}

void Renderer::draw(const Circle& object) const {
	SDL_Vertex vertices[3]{};
	float radiansPerSegment = 2.0f * (float)M_PI / segments_per_object;
	float circleX = object.position.get_x();
	float circleY = object.position.get_y();
	float circleRadius = object.radius;
	for (int i = 0;i < this->segments_per_object;i++) {
		float radiansA = i * radiansPerSegment;
		float radiansB = (i + 1) * radiansPerSegment;

		float Ax = circleX + circleRadius * cosf(radiansA);
		float Ay = circleY + circleRadius * sinf(radiansA);
		float Bx = circleX + circleRadius * cosf(radiansB);
		float By = circleY + circleRadius * sinf(radiansB);

		unsigned char red = object.color.r;
		unsigned char green = object.color.g;
		unsigned char blue = object.color.b;

		vertices[0] = { {circleX, circleY}, {red, green, blue} };
		vertices[1] = { {Ax, Ay}, {red, green, blue} };
		vertices[2] = { {Bx, By}, {red, green, blue} };

		SDL_RenderGeometry(this->sdl_renderer, nullptr, vertices, 3, nullptr, 0);
	}
}
