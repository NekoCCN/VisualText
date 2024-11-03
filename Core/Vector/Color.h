#pragma once
#ifndef VISUALTEXT_CORE_VECTOR_COLOR_H_
#define VISUALTEXT_CORE_VECTOR_COLOR_H_
#include <stdint.h>

namespace vtcore
{
	class RgbColorFloat
	{
	private:
		float color_[4]{ 0, 0, 0, 1 };
	public:
		RgbColorFloat() = default;
		RgbColorFloat(float r, float g, float b, float a)
		{
			color_[0] = r;
			color_[1] = g;
			color_[2] = b;
			color_[3] = a;
		}
		float* getColorArray()
		{
			return color_;
		}
		float& operator[](uint8_t num)
		{
			return color_[num];
		}
		void setRGBA(float r, float g, float b, float a)
		{
			color_[0] = r;
			color_[1] = g;
			color_[2] = b;
			color_[3] = a;
		}
		void setRGB(float r, float g, float b)
		{
			color_[0] = r;
			color_[1] = g;
			color_[2] = b;
		}
	};

	class RgbColorInt
	{
	private:
		uint8_t color_[4]{ 0, 0, 0, 1 };
	public:
		RgbColorInt() = default;
		RgbColorInt(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			color_[0] = r;
			color_[1] = g;
			color_[2] = b;
			color_[3] = a;
		}
		uint8_t* getColorArray()
		{
			return color_;
		}
		uint8_t& operator[](uint8_t num)
		{
			return color_[num];
		}
		void setRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			color_[0] = r;
			color_[1] = g;
			color_[2] = b;
			color_[3] = a;
		}
		void setRGB(uint8_t r, uint8_t g, uint8_t b)
		{
			color_[0] = r;
			color_[1] = g;
			color_[2] = b;
		}
	};
}
#endif