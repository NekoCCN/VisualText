#pragma once
#ifndef VISUALTEXT_CORE_VECTOR_RECT_H_
#define VISUALTEXT_CORE_VECTOR_RECT_H_
#include "RatioRect.h"

vtcore::RatioRect::RatioRect(Window& window, DisplayRectRatio dpr) 
	: display_rect_(window.getWindowRect()), display_rect_ratio_(dpr)
{  }
vtcore::RatioRect::RatioRect(SDL_Rect sr, DisplayRectRatio dpr)
	: display_rect_ratio_(dpr), display_rect_(sr)
{  }
SDL_Rect vtcore::RatioRect::getIntRect()
{
	SDL_Rect tmp_rect{};
	tmp_rect.x = (int)(display_rect_.x * display_rect_ratio_.x);
	tmp_rect.y = (int)(display_rect_.y * display_rect_ratio_.y);
	tmp_rect.w = (int)(display_rect_.w * display_rect_ratio_.w);
	tmp_rect.h = (int)(display_rect_.h * display_rect_ratio_.h);
	return tmp_rect;
}
SDL_FRect vtcore::RatioRect::operator()()
{
	SDL_FRect tmp_rect{};
	tmp_rect.x = (float)(display_rect_.x * display_rect_ratio_.x);
	tmp_rect.y = (float)(display_rect_.y * display_rect_ratio_.y);
	tmp_rect.w = (float)(display_rect_.w * display_rect_ratio_.w);
	tmp_rect.h = (float)(display_rect_.h * display_rect_ratio_.h);
	return tmp_rect;
}
SDL_FRect vtcore::RatioRect::getFloatRect()
{
	SDL_FRect tmp_rect{};
	tmp_rect.x = (float)(display_rect_.x * display_rect_ratio_.x);
	tmp_rect.y = (float)(display_rect_.y * display_rect_ratio_.y);
	tmp_rect.w = (float)(display_rect_.w * display_rect_ratio_.w);
	tmp_rect.h = (float)(display_rect_.h * display_rect_ratio_.h);
	return tmp_rect;
}
#endif