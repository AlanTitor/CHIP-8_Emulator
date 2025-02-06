#pragma once

#include <cstdint>
#include <SDL2/SDL.h>


class Graphic{
public:
	Graphic(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
	~Graphic();
	void Update(void const* buffer, int pitch);
	bool ProcessInput(uint8_t* keys);

private:
	SDL_Window* window{};
	SDL_GLContext gl_context{};
	SDL_Renderer* renderer{};
	SDL_Texture* texture{};
};