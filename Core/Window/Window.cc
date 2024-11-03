#include "Window.h"

vtcore::Window::Window(const char* title, uint32_t width, uint32_t height, SDL_WindowFlags flags, const char* drive)
{
	// window part
	lst.logIn("Try to create a window", logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_VIDEO);
	window_ = SDL_CreateWindow(title, width, height, flags);
	if (window_ == nullptr)
	{
		lst.logIn("Failed to create window", logsys::LOG_PRIORITY_CRITICAL, logsys::LOG_CATEGORY_VIDEO);
		throw create_window_error();
	}
	// renderer part
	drive_num_ = SDL_GetNumRenderDrivers();
	lst.logIn("Supported Graphic API List: ", logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_VIDEO);
	for (int i = 0; i < drive_num_; ++i)
	{
		render_drive_str_.push_back(std::string(SDL_GetRenderDriver(i)));
		lst.logIn(render_drive_str_[i], logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_VIDEO);
	}
	lst.logIn("END\n", logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_VIDEO);
	sdl_renderer_ = SDL_CreateRenderer(window_, drive);
	if (sdl_renderer_ == nullptr)
	{
		lst.logIn("Failed to create window", logsys::LOG_PRIORITY_CRITICAL, logsys::LOG_CATEGORY_VIDEO);
		throw create_window_error();
	}
	SDL_ShowWindow(window_);
}