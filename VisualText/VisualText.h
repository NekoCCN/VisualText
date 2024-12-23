﻿#pragma once
#ifndef VISUALTEXT_VISUALTEXT_VISUALTEXT_H_
#define VISUALTEXT_VISUALTEXT_VISUALTEXT_H_
#include <imgui.h>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <AssetManager/AssetPack/AssetPack.h>
#include <AssetManager/AssetPackStream/AssetPackStream.h>
#include <AssetManager/BinaryPack/BinaryPack.h>
#include <AssetManager/BinaryPackGenerate/ThemeTemplateGenerate/ThemeTemplateGenerate.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <Core/Exception/Exception.h>
#include <Core/LogSystem/LogSystem.h>
#include <Core/Transform/Transform.h>
#include <Core/Vector/RatioRect.h>
#include <Core/Window/Window.h>
#include <Render/RenderFunction/RenderFunction.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <Texture/Font/Font.h>
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
		set_terminate(vtcore::exception_hinding);
	}
	void init_SDL_image();
	void init_SDL()
	{
		using namespace vtcore;
		using namespace logsys;
		if (!SDL_InitSubSystem(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD))
		{
			vtcore::lst.logIn("Window system initialization failed", LOG_PRIORITY_CRITICAL, LOG_CATEGORY_APPLICATION);
			vtcore::lst.logIn(SDL_GetError(), LOG_PRIORITY_CRITICAL, LOG_CATEGORY_APPLICATION);
			throw SDL_init_error();
		}
		else
		{
			vtcore::lst.logIn("Window system initialization successful", LOG_PRIORITY_INFO, LOG_CATEGORY_APPLICATION);
		}
	}
	void init_SDL_TTF()
	{
		using namespace vtcore;
		using namespace logsys;
		if (!TTF_Init())
		{
			vtcore::lst.logIn("TrueType system initialization failed", LOG_PRIORITY_CRITICAL, LOG_CATEGORY_APPLICATION);
			vtcore::lst.logIn(SDL_GetError(), LOG_PRIORITY_CRITICAL, LOG_CATEGORY_APPLICATION);
			throw SDL_init_error();
		}
		else
		{
			vtcore::lst.logIn("Window system initialization successful", LOG_PRIORITY_INFO, LOG_CATEGORY_APPLICATION);
		}
	}
	void init()
	{
		init_exception_hinding();
		vtcore::logsys::initLogSys();
		
		init_SDL();
		init_SDL_image();
		init_SDL_TTF();
	}
public:
	ProjectInitializer()
	{
		init();
	}
	ProjectInitializer(const char* name)
	{
		vtcore::lst.enableFile(name);
		init();
	}
	ProjectInitializer(bool st)
	{
		if (st == true)
			vtcore::lst.enableFile();
		init();
	}
	~ProjectInitializer()
	{
		IMG_Quit();
		SDL_Quit();
	}
};

class Application
{
private:
	std::vector< std::shared_ptr<std::thread> > asset_threads;
	std::vector< std::shared_ptr<std::thread> > audio_threads;
public:
};
#endif
