#pragma once
#ifndef VISUALTEXT_ASSETMANAGER_BINARYPACK_BINARYPACK_H_
#define VISUALTEXT_ASSETMANAGER_BINARYPACK_BINARYPACK_H_
#include <SDL3/SDL.h>
#include <Core/LogSystem/LogSystem.h>
#include <Core/Exception/Exception.h>
#include "../MemoryBuffer/MemoryBuffer.h"
#include <fstream>
#include <string>
#include <utility>

namespace vtasset
{
	class BinaryPack
	{
	private:
		uint32_t file_num_ = 0;
		uint64_t* toc_ = nullptr;
		static const uint32_t label_offset = 22;
		uint64_t resource_offset_ = 0;
		std::ifstream fs_;
		bool is_ready_;
	public:
		BinaryPack() : is_ready_(false) {  }
		BinaryPack(const std::string& path);
		BinaryPack(std::ifstream& fs);
		bool isReady()
		{
			return is_ready_;
		}
		bool loadToBuffer_s(uint32_t index, char* buffer, uint64_t buffer_size);
		bool loadToBuffer(uint32_t index, char* buffer);
		uint32_t getFileNum(uint32_t index) const;
		uint64_t getBufferSize(uint32_t index) const;
		char* operator[](uint32_t index);
		bool open(const std::string& path);
		bool open(std::ifstream& fs);
	};
}
#endif