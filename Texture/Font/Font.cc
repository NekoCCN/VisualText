#include "Font.h"
#include <Core/LogSystem/LogSystem.h>

vttexture::Font::Font(SDL_IOStream* iostream, float ptsize, bool closeio)
{
	font_ = TTF_OpenFontIO(iostream, closeio, ptsize);
	if (font_ == nullptr)
	{
		vtcore::lst.logIn("Font creation failed, maybe can not open font file");
	}
}
vttexture::Font::Font(vtasset::BinaryPack& BP, uint32_t index, float ptsize)
{
	char* buffer = BP.operator[](index);
	SDL_IOStream* iostream = SDL_IOFromMem((void*)buffer, BP.getBufferSize(index));
	font_ = TTF_OpenFontIO(iostream, true, ptsize);
	if (font_ == nullptr)
	{
		vtcore::lst.logIn("Font creation failed, maybe can not open font file", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_VIDEO);
		vtcore::lst.logIn(SDL_GetError(), vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_VIDEO);
	}
	// maybe should not releases
}
SDL_Surface* vttexture::Font::getTextSurface_Blended(const char* text, size_t length, SDL_Color foreground, int32_t wrap_length)
{
	if (wrap_length != 0)
	{
		SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font_, text, length, foreground, wrap_length);
		if (surface == nullptr)
		{
			vtcore::lst.logIn("Can not render text.", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_RENDER);
			vtcore::lst.logIn(SDL_GetError(), vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_RENDER);
		}
			return surface;
	}
	else
	{
		SDL_Surface* surface = TTF_RenderText_Blended(font_, text, length, foreground);
		if (surface == nullptr)
		{
			vtcore::lst.logIn("Can not render text.", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_RENDER);
			vtcore::lst.logIn(SDL_GetError(), vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_RENDER);
		}
		return surface;
	}
}
SDL_Surface* vttexture::Font::getTextSurface_LCD(const char* text, size_t length, SDL_Color foreground, SDL_Color background, int32_t wrap_length)
{
	if (wrap_length != 0)
	{
		SDL_Surface* surface = TTF_RenderText_LCD_Wrapped(font_, text, length, foreground, background, wrap_length);
		if (surface == nullptr)
			vtcore::lst.logIn("Can not render text.", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_RENDER);
		return surface;
	}
	else
	{
		SDL_Surface* surface = TTF_RenderText_LCD(font_, text, length, foreground, background);
		if (surface == nullptr)
			vtcore::lst.logIn("Can not render text.", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_RENDER);
		return surface;
	}
}
SDL_Surface* vttexture::Font::getTextSurface_Shaded(const char* text, size_t length, SDL_Color foreground, SDL_Color background, int32_t wrap_length)
{
	if (wrap_length != 0)
	{
		SDL_Surface* surface = TTF_RenderText_Shaded_Wrapped(font_, text, length, foreground, background, wrap_length);
		if (surface == nullptr)
			vtcore::lst.logIn("Can not render text.", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_RENDER);
		return surface;
	}
	else
	{
		SDL_Surface* surface = TTF_RenderText_Shaded(font_, text, length, foreground, background);
		if (surface == nullptr)
			vtcore::lst.logIn("Can not render text.", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_RENDER);
		return surface;
	}
}