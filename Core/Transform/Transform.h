#include <SDL3/SDL.h>
#include <memory>

namespace vtcore
{
	namespace transform
	{
		void gaussian_blur(SDL_Surface* src, SDL_Surface* dst, int sigma, bool repeat);
		void box_blur(SDL_Surface* src, SDL_Surface* dst, int radius, bool repeat);
	}
}