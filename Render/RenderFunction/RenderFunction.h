#pragma once
#ifndef _VISUALTEXT_RENDER_RENDER_FUNCTION_H_
#define _VISUALTEXT_RENDER_RENDER_FUNCTION_H_

#include <cstdint>
#include <SDL3/SDL.h>
#include <Core/Transform/Transform.h>
#include <Core/LogSystem/LogSystem.h>
#include <Core/Exception/Exception.h>
#include <Core/Monitor/Monitor.h>
#include <Core/Window/Window.h>
#include <Core/PointFunction/PointFunction.h>

namespace vtrender
{
	class RenderFunction
	{
	private:
		vtcore::PointFunction pf_;
	public:
		// must be called after other render function and before Font render function
		bool renderMainTextBoxUsingGaussianBlur(vtcore::Window& win, uint32_t index);
	};
}

#endif