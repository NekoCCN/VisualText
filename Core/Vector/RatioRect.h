#pragma once
#ifndef VISUALTEXT_CORE_VECTOR_RATIORECT_H_
#define VISUALTEXT_CORE_VECTOR_RATIORECT_H_
#include <SDL3/SDL_rect.h>
#include <cstdint>
#include "../Window/Window.h"

namespace vtcore
{
	class RatioRect
	{
	public:
		struct DisplayRectRatio
		{
			double x, y;
			double w, h;
		};
	private:
		SDL_Rect display_rect_;
		DisplayRectRatio display_rect_ratio_;
	public:
		RatioRect() : display_rect_({ 0, 0, 0, 0 }), display_rect_ratio_({ 0, 0, 0, 0 }) {  }
		RatioRect(Window& window, DisplayRectRatio dpr = { 0.5, 0.5, 0.6, 0.3 });
		RatioRect(SDL_Rect sr, DisplayRectRatio dpr = { 0.5, 0.5, 0.6, 0.3 });
		SDL_Rect getIntRect();  // Follow SDL's rectangular rules from top left to bottom right
		SDL_FRect operator()();  // Follow SDL's rectangular rules from top left to bottom right
		SDL_FRect getFloatRect();
		void resetDisplayRectRatio(DisplayRectRatio dpr)
		{
			display_rect_ratio_ = dpr;
		}
		void resetDisplayRect(SDL_Rect dr)
		{
			display_rect_ = dr;
		}
		void setRatioX(double x)
		{
			display_rect_ratio_.x = x;
		}
		void setRatioY(double y)
		{
			display_rect_ratio_.y = y;
		}
		void setRatioW(double w)
		{
			display_rect_ratio_.w = w;
		}
		void setRatioH(double h)
		{
			display_rect_ratio_.h = h;
		}
	};
}
#endif