#pragma once
#ifndef VISUALTEXT_CORE_TEXTURE_TEXTURE_H_
#define VISUALTEXT_CORE_TEXTURE_TEXTURE_H_
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <Core/Window/Window.h>

class Texture
{
private:
	SDL_Texture* texture_ = nullptr;
	Window& texture_window_;
public:
	Texture(Window& window) : texture_window_(window) {  }
	Texture(Window& window, SDL_Surface* surface) : texture_window_(window)
	{
		texture_ = SDL_CreateTextureFromSurface(texture_window_.getRendererHinding(), surface);
		if (texture_ == nullptr)
		{
			lst.logIn("Fail to load a texture", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_RENDER);
			lst.logIn(SDL_GetError(), logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_RENDER);
		}
	}
	bool isWindow(Window& window)
	{
		return (&window) == (&texture_window_);
	}
	~Texture()
	{
		SDL_DestroyTexture(texture_);
	}
};

#endif

