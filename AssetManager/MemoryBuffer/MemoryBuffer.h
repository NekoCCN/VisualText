#pragma once
#ifndef VISUALTEXT_ASSETMANAGER_MEMORYBUFFER_MEMORYBUFFER_H_
#define VISUALTEXT_ASSETMANAGER_MEMORYBUFFER_MEMORYBUFFER_H_

#include <Core/LogSystem/LogSystem.h>
#include <memory>

namespace vtasset
{
	class MemoryBuffer
	{
	private:
		std::shared_ptr<char> buffer_ = nullptr;
		uint64_t byte_size_ = 0;
	public:
		friend class AssetPack;
		MemoryBuffer() = default;
		MemoryBuffer(std::shared_ptr<char> buffer, uint64_t byte_size) noexcept
		{
			buffer_ = buffer;
			byte_size_ = byte_size;
		}
		MemoryBuffer(uint64_t byte_size) noexcept
		{
			byte_size_ = byte_size;
			buffer_ = std::shared_ptr<char>(new char[byte_size_]);
		}
		MemoryBuffer(MemoryBuffer&& buffer) noexcept
		{
			buffer_ = buffer.buffer_;
			buffer.buffer_ = nullptr;
			byte_size_ = buffer.byte_size_;
			buffer.byte_size_ = 0;
		}
		~MemoryBuffer()
		{  }
		uint64_t getBufferByte()
		{
			return byte_size_;
		}
		std::shared_ptr<char> getBufferPoint()
		{
			return buffer_;
		}
		MemoryBuffer& operator=(MemoryBuffer&& buffer) noexcept
		{
            buffer_ = buffer.buffer_;
			buffer.buffer_ = nullptr;
			byte_size_ = buffer.byte_size_;
			buffer.byte_size_ = 0;
			return *this;
		}
		bool isEmpty()
		{
			return ( buffer_ == nullptr || byte_size_ == 0 );
		}
	};
}

#endif