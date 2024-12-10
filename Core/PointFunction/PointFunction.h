/**
* @author Ext Culith
**/

#pragma once
#ifndef VISUALTEXT_CORE_POINTFUNCTION_H
#define VISUALTEXT_CORE_POINTFUNCTION_H
#include "../Monitor/Monitor.h"
#include <SDL3/SDL.h>
#include <cstdint>

namespace vtcore
{
	class PointFunction
	{
	private:
		MonitorLists monitor_lists_;
	public:
		SDL_Rect getMainTextBoxRect(uint32_t index);
		SDL_Point getMainTextBoxTextPoint(uint32_t index);
		
	};
}
#endif // VISUALTEXT_CORE_POINTFUNCTION_H
