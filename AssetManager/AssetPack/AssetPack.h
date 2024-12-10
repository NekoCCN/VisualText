#pragma once
#ifndef VISUALTEXT_ASSETMANAGER_ASSETPACK_ASSETPACK_H
#define VISUALTEXT_ASSETMANAGER_ASSETPACK_ASSETPACK_H
#include <Core/LogSystem/LogSystem.h>
#include <vector>
#include <cstdint>
#include "../AssetPackStream/AssetPackStream.h"
#include "../MemoryBuffer/MemoryBuffer.h"

namespace vtasset
{
	struct ExplicitNode
	{
		uint32_t index;
		std::string name;
		std::vector<ExplicitNode> children;
	};
	class AssetPack
	{
	private:
		ProgramIndex* program_index_list_;
		uint64_t* node_list_;
		AssetStruct* toc_;  // long offset = 4GB, long long offset = 17179869184GB = 16777216TB

		std::vector<ExplicitNode> explicit_node_list_;

		ProgramIndex* program_index_pointer_;

		uint64_t index_offset_;
		uint64_t resources_offset_;

		uint64_t program_index_num_;
		uint32_t node_num_;
		uint32_t toc_num_;

		std::ifstream fs_;
		bool is_ready_ = false;
		std::string label_ = "VisualTextAssetPack;";
	public:
		AssetPack(const std::string& path);
		AssetPack(const AssetPack& as) = delete;
		AssetPack(AssetPack&& as) = default;
		AssetPack& operator=(const AssetPack& as) = delete;
		AssetPack& operator=(AssetPack&& as) = default;
		~AssetPack()
		{
			delete[] program_index_list_;
			delete[] node_list_;
			delete[] toc_;
		}
		uint64_t getFileByte(uint32_t index) const;
		AssetPack& operator>>(ProgramIndex& pi);
		bool goProgramIndex(uint32_t index);
		bool getMemoryBuffer(uint32_t index, MemoryBuffer& m_buffer);
	};
}

#endif