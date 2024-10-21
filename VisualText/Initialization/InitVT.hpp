#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <Core/Exception/Exception.h>
#include <Core/LogSystem/LogSystem.h>

class ProjectIniter
{
private:
	bool AdditionalImageSupport = true;
	bool WebpSupport = true;
	void init_exception_hinding()
	{
		set_terminate(exception_hinding);
	}
	void init_SDL_image()
	{
		using namespace LogSystem;
		if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_AVIF | IMG_INIT_JXL | IMG_INIT_TIF | IMG_INIT_WEBP))
		{
			lst.logIn("Image loader init additional format error", WARN, VIDEO);
			AdditionalImageSupport = false;
			if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP))
			{
				lst.logIn("Image loader init WEBP format error", WARN, VIDEO);
				WebpSupport = false;
				if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))
				{
					lst.logIn("Image loader init error", FATAL, VIDEO);
					throw SDL_init_error();
				}
				else
					lst.logIn("Image loader init incomplete initialization", WARN, VIDEO);
			}
			else
				lst.logIn("Image loader init incomplete initialization", WARN, VIDEO);
		}
		else
			lst.logIn("Image loader init initialization successful", WARN, VIDEO);
	}
	void init_SDL()
	{
		using namespace LogSystem;
		if (!SDL_InitSubSystem(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMEPAD))
		{
			lst.logIn("Window system initialization failed", FATAL, VIDEO);
			lst.logIn(SDL_GetError(), FATAL, VIDEO);
			throw SDL_init_error();
		}
		else
		{
			lst.logIn("Window system initialization successful", INFO, VIDEO);
		}
	}
	void init()
	{
		using namespace LogSystem;
		init_exception_hinding();
		init_SDL();
		init_SDL_image();
	}
public:
	ProjectIniter()
	{
		init();
	}
	ProjectIniter(const char* name)
	{
		lst.enableFile(name);
		init();
	}
	ProjectIniter(bool st)
	{
		if (st == true)
			lst.enableFile();
		init();
	}
	~ProjectIniter()
	{
		IMG_Quit();
		SDL_Quit();
	}
};