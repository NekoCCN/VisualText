#pragma once
#ifndef VISUALTEXT_CORE_WINDOW_WINDOW_H_
#define VISUALTEXT_CORE_WINDOW_WINDOW_H_
#include <SDL3/SDL.h>
#include <Core/LogSystem/LogSystem.h>
#include <Core/Exception/Exception.h>
#include <Core/Monitor/Monitor.h>
#include <string>
#include <vector>

class Window
{
public:
	Window(const char* title, uint32_t width = 1280, uint32_t height = 720, SDL_WindowFlags flags = SDL_WINDOW_HIDDEN, const char* drive = "opengl");
	~Window()
	{
		SDL_DestroyRenderer(sdl_renderer_);
		SDL_DestroyWindow(window_);
	}
	bool showWindow()  // maybe thread safe
	{
		if (!SDL_ShowWindow(window_))
			return false;
		return true;
	}
	bool hideWindow()  // maybe thread safe
	{
		if (!SDL_HideWindow(window_))
			return false;
		return true;
	}
	bool presentRender()
	{
		if (!SDL_RenderPresent(sdl_renderer_))
		{
			lst.logIn("Can not render", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_RENDER);
			lst.logIn(SDL_GetError(), logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_RENDER);
			return false;
		}
		return true;
	}
	SDL_Renderer* getRendererHinding()
	{
		return sdl_renderer_;
	}
	SDL_Window* getWindowHinding()
	{
		return window_;
	}
	bool setRenderVSync(int32_t vsync)
	{
		if (!SDL_SetRenderVSync(sdl_renderer_, vsync))
		{
			lst.logIn("Fail to set render vsync", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_RENDER);
			lst.logIn(SDL_GetError(), logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_RENDER);
			return false;
		}
		return true;
	}
	void setWindowPosition(int32_t x = SDL_WINDOWPOS_CENTERED, int32_t y = SDL_WINDOWPOS_CENTERED)
	{
		SDL_SetWindowPosition(window_, x, y);
	}
	bool isMinimized()
	{
		return (SDL_GetWindowFlags(window_) & SDL_WINDOW_MINIMIZED);
	}
	bool setRenderDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		SDL_SetRenderDrawColor(sdl_renderer_, r, g, b, a);
	}
	bool setRenderDrawColor(float r, float g, float b, float a)
	{
		SDL_SetRenderDrawColorFloat(sdl_renderer_, r, g, b, a);
	}
	void renderClear()
	{
		SDL_RenderClear(sdl_renderer_);
	}
private:
	// Surface-based software rendering is no longer used by default.
	// Software rendering cannot be used with hardware rendering
	std::vector<std::string> render_drive_str_;
	uint32_t drive_num_;
	SDL_Renderer* sdl_renderer_;
	SDL_Window* window_;
};
#endif