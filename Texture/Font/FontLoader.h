#pragma once
#ifndef VISUALTEXT_CORE_TEXTURE_FONTLOADER_FONTLOADER_H_
#define VISUALTEXT_CORE_TEXTURE_FONTLOADER_FONTLOADER_H_
#include <SDL3_ttf/SDL_ttf.h>

class FontLoader
{
private:
	TTF_TextEngine* text_engine_;
public:
	FontLoader();
};

#endif