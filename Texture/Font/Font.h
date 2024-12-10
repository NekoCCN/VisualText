#pragma once
#ifndef VISUALTEXT_TEXTURE_FONT_FONT_H
#define VISUALTEXT_TEXTURE_FONT_FONT_H
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <AssetManager/BinaryPack/BinaryPack.h>

namespace vttexture
{
	class Font
	{
	private:
		TTF_Font* font_ = nullptr;
		char* buffer_;
	public:
		Font(SDL_IOStream* iostream, float ptsize, bool closeio = true);
		Font(Font&) = default;
		Font& operator=(const Font& op)
		= default;

		Font(Font&& op) noexcept
		{
			buffer_ = op.buffer_;
			op.buffer_ = nullptr;
			font_ = op.font_;
			op.font_ = nullptr;
		}
		Font& operator=(Font&& op) noexcept
		{
			buffer_ = op.buffer_;
			op.buffer_ = nullptr;
			font_ = op.font_;
			op.font_ = nullptr;
			return *this;
		}
		Font(vtasset::BinaryPack& bp, uint32_t index, float ptsize);
		SDL_Surface* getTextSurface_Blended(const char* text, SDL_Color foreground = { 0, 0, 0, 1 }, int32_t wrap_length = 0, size_t length = 0) const;
		SDL_Surface* getTextSurface_LCD(const char* text, SDL_Color foreground = { 0, 0, 0, 1 }, SDL_Color background = { 255, 255, 255, 0 }, int32_t wrap_length = 0, size_t length = 0) const;
		SDL_Surface* getTextSurface_Shaded(const char* text, SDL_Color foreground = { 0, 0, 0, 1 }, SDL_Color background = { 255, 255, 255, 0 }, int32_t wrap_length = 0, size_t length = 0) const;
		~Font()
		{
			delete[] buffer_;
		}
	};
}
#endif // VISUALTEXT_TEXTURE_FONT_FONT_H
