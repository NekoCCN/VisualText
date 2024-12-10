#include "PointFunction.h"

SDL_Rect vtcore::PointFunction::getMainTextBoxRect(uint32_t index)
{
	monitor_lists_.refresh();
    SDL_Rect display_rect = monitor_lists_.getDisplayBound(index);
	SDL_Rect res_rect;
	res_rect.x = display_rect.x * 0.05;
	res_rect.y = display_rect.y * 0.6;
	res_rect.w = display_rect.w * 0.85;
    res_rect.h = display_rect.h * 0.35;
	return res_rect;
}
SDL_Point vtcore::PointFunction::getMainTextBoxTextPoint(uint32_t index)
{
	monitor_lists_.refresh();
	SDL_Rect display_rect = monitor_lists_.getDisplayBound(index);
	SDL_Point point;
	point.x = display_rect.x * 0.1;
	point.y = display_rect.y * 0.65;
	return point;
}
