#include "RenderFunction.h"

bool vtrender::RenderFunction::renderMainTextBoxUsingGaussianBlur(vtcore::Window& win, uint32_t index)
{
	SDL_Rect render_rect = pf_.getMainTextBoxRect(index);
	SDL_Rect dst_rect{ 0, 0, render_rect.w, render_rect.h };

	SDL_Surface* current_surface = win.getWindowSurfaceCopy();

	SDL_Surface* src = SDL_CreateSurface(render_rect.w, render_rect.h, current_surface->format);
	SDL_Surface* dst = SDL_CreateSurface(render_rect.w, render_rect.h, current_surface->format);

	if (!SDL_BlitSurface(current_surface, &render_rect, src, &dst_rect))
	{
		SDL_DestroySurface(current_surface);
		SDL_DestroySurface(src);
		SDL_DestroySurface(dst);
		return false;
	}
	vtcore::transform::gaussian_blur(src, dst, 5, true);

	SDL_Texture* texture = win.surfaceToTexture(dst);

	SDL_FRect render_frect { static_cast<float>(render_rect.x), static_cast<float>(render_rect.y),
		static_cast<float>(render_rect.w), static_cast<float>(render_rect.h) };
	SDL_FRect dst_frect { 0, 0, static_cast<float>(render_rect.w), static_cast<float>(render_rect.h) };
	win.renderTexture(texture, &render_frect, &dst_frect);

	SDL_DestroySurface(current_surface);
	SDL_DestroySurface(src);
	SDL_DestroySurface(dst);
    SDL_DestroyTexture(texture);

	return true;
}