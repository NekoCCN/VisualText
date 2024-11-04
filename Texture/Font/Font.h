#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <AssetManager/BinaryPack/BinaryPack.h>

namespace vttexture
{
	class Font
	{
	private:
		TTF_Font* font_ = nullptr;
	public:
		Font(SDL_IOStream* iostream, float ptsize, bool closeio = true);
		Font(vtasset::BinaryPack& BP, uint32_t index, float ptsize);
		SDL_Surface* getTextSurface_Blended(const char* text, SDL_Color foreground = { 0, 0, 0, 1 }, int32_t wrap_length = 0, size_t length = 0);
		SDL_Surface* getTextSurface_LCD(const char* text, SDL_Color foreground = { 0, 0, 0, 1 }, SDL_Color background = { 255, 255, 255, 0 }, int32_t wrap_length = 0, size_t length = 0);
		SDL_Surface* getTextSurface_Shaded(const char* text, SDL_Color foreground = { 0, 0, 0, 1 }, SDL_Color background = { 255, 255, 255, 0 }, int32_t wrap_length = 0, size_t length = 0);
	};
}