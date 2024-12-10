#pragma once
#ifndef VISUALTEXT_CORE_VECTOR_RECT_H_
#define VISUALTEXT_CORE_VECTOR_RECT_H_
#include "RatioRect.h"

vtcore::RatioRect::RatioRect(Window& window, DisplayRectRatio dpr) 
	: display_rect_(window.getWindowRect()), display_rect_ratio_(dpr)
{ 
	display_rect_.x = display_rect_.w;
	display_rect_.y = display_rect_.h;
}
vtcore::RatioRect::RatioRect(SDL_Rect sr, DisplayRectRatio dpr)
	: display_rect_ratio_(dpr), display_rect_(sr)
{
	display_rect_.x = display_rect_.w;
	display_rect_.y = display_rect_.h;
}
SDL_Rect vtcore::RatioRect::getIntRect(int w, int h) const
{
	SDL_Rect tmp_rect{};
	tmp_rect.x = static_cast<int>(display_rect_.x * display_rect_ratio_.x);
	tmp_rect.y = static_cast<int>(display_rect_.y * display_rect_ratio_.y);
	if (w != 0)
		tmp_rect.w = w;
	else
		tmp_rect.w = static_cast<int>(display_rect_.w * display_rect_ratio_.w);
	if (h != 0)
		tmp_rect.h = h;
	else
		tmp_rect.h = static_cast<int>(display_rect_.h * display_rect_ratio_.h);
	return tmp_rect;
}
SDL_FRect vtcore::RatioRect::operator()(float w, float h) const
{
	SDL_FRect tmp_rect{};
	tmp_rect.x = static_cast<float>(display_rect_.x * display_rect_ratio_.x);
	tmp_rect.y = static_cast<float>(display_rect_.y * display_rect_ratio_.y);
	if (w != 0)
		tmp_rect.w = w;
	else
		tmp_rect.w = static_cast<float>(display_rect_.w * display_rect_ratio_.w);
	if (h != 0)
		tmp_rect.h = h;
	else
		tmp_rect.h = static_cast<float>(display_rect_.h * display_rect_ratio_.h);
	return tmp_rect;
}
SDL_FRect vtcore::RatioRect::getFloatRect(float w, float h) const
{
	SDL_FRect tmp_rect{};
	tmp_rect.x = static_cast<float>(display_rect_.x * display_rect_ratio_.x);
	tmp_rect.y = static_cast<float>(display_rect_.y * display_rect_ratio_.y);
	if (w != 0)
		tmp_rect.w = w;
	else
		tmp_rect.w = static_cast<float>(display_rect_.w * display_rect_ratio_.w);
	if (h != 0)
		tmp_rect.h = h;
	else
		tmp_rect.h = static_cast<float>(display_rect_.h * display_rect_ratio_.h);
	return tmp_rect;
}
#endif