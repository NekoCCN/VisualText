#pragma once
#ifndef VISUALTEXT_ASSETMANAGER_MEMORYBUFFER_MEMORYBUFFER_H_
#define VISUALTEXT_ASSETMANAGER_MEMORYBUFFER_MEMORYBUFFER_H_

#include <Core/LogSystem/LogSystem.h>

namespace vtasset
{
	class MemoryBuffer
	{
	private:
		char* buffer_ = nullptr;
		uint64_t byte_size_;
	public:
		MemoryBuffer() = default;
		MemoryBuffer(char* buffer, uint64_t byte_size)
		{
			buffer_ = buffer;
			byte_size_ = byte_size;
		}
		MemoryBuffer(uint64_t byte_size)
		{
			byte_size_ = byte_size;
			buffer_ = new char[byte_size_];
		}
		~MemoryBuffer()
		{
			delete[] buffer_;
		}
		uint64_t getBufferByte()
		{
			return byte_size_;
		}
		void refreshMemoryBuffer(char* buffer, uint64_t byte_size)
		{
			if (buffer_ != nullptr)
				delete[] buffer_;
			buffer_ = buffer;
			byte_size_ = byte_size;
		}
		char* getBufferPoint()
		{
			return buffer_;
		}
	};
}

#endif