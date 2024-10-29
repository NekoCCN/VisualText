#pragma once
#ifndef VISUALTEXT_VISUALTEXT_VISUALTEXT_H_
#define VISUALTEXT_VISUALTEXT_VISUALTEXT_H_
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <Core/Exception/Exception.h>
#include <AssetManager/BinaryPackGenerate/ThemeTemplateGenerate/ThemeTemplateGenerate.h>
#include <AssetManager/BinaryPack/BinaryPack.h>
#include <Core/LogSystem/LogSystem.h>
#include <Core/Window/Window.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

class ProjectInitializer
{
private:
	bool additional_image_support_ = true;
	bool webp_support_ = true;
	void init_exception_hinding()
	{
		set_terminate(exception_hinding);
	}
	void init_SDL_image();
	void init_SDL()
	{
		using namespace logsys;
		if (!SDL_InitSubSystem(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD))
		{
			lst.logIn("Window system initialization failed", LOG_PRIORITY_CRITICAL, LOG_CATEGORY_APPLICATION);
			lst.logIn(SDL_GetError(), LOG_PRIORITY_CRITICAL, LOG_CATEGORY_APPLICATION);
			throw SDL_init_error();
		}
		else
		{
			lst.logIn("Window system initialization successful", LOG_PRIORITY_INFO, LOG_CATEGORY_APPLICATION);
		}
	}
	void init()
	{
		init_exception_hinding();
		logsys::initLogSys();
		init_SDL();
		init_SDL_image();
	}
public:
	ProjectInitializer()
	{
		init();
	}
	ProjectInitializer(const char* name)
	{
		lst.enableFile(name);
		init();
	}
	ProjectInitializer(bool st)
	{
		if (st == true)
			lst.enableFile();
		init();
	}
	~ProjectInitializer()
	{
		IMG_Quit();
		SDL_Quit();
	}
};

#endif
