#pragma once
#ifndef VISUALTEXT_ASSETMANAGER_ASSETPACKSTREAM_ASSETPACKSTREAM_H
#define VISUALTEXT_ASSETMANAGER_ASSETPACKSTREAM_ASSETPACKSTREAM_H
#include <Core/Exception/Exception.h>
#include <Core/LogSystem/LogSystem.h>
#include <Core/CommandList/CommandList.h>
#include "../AssetFormat/AssetFormat.h"
#include <SDL3/SDL_storage.h>
#include <map>
#include <vector>
#include <cstdint>
#include <cstring>
#include <sstream>

namespace vtasset
{
	struct AssetStruct  // TOC List, Change with caution
	{
		assetformat::AssetFormat asset_format = assetformat::ASSET_FORMAT_FILE;
		uint32_t index = 0;  
		uint64_t toc_offset = 0;
	};
	struct ProgramIndexPushStruct  // Program Command List, Change with caution
	{
		vtcore::command::CommandList command = vtcore::command::error_null;

		bool is_node = false;
		bool is_node_hide = false;
		char node_name[32]{};

		uint32_t command_argument[3]{};

		assetformat::AssetFormat asset_format_list[10]{};

		std::string asset_filename_list[10]{};  // File Mode
		uint16_t asset_list_index_size = 0;

		std::string string;  // String Mode	
	};
	struct ProgramIndex
	{
		bool is_node = false;
		bool is_node_hide = false;
		char node_name[32]{};

		uint32_t command_argument[3]{};
		uint32_t asset_list_index[10]{};
		uint16_t asset_list_index_size = 0;
		vtcore::command::CommandList command = vtcore::command::error_null;
	};
	class AssetPackStream
	{
	private:
		std::vector<ProgramIndex> program_index_list_;
		std::vector<uint64_t> node_list_;
		std::vector<AssetStruct> toc_;  // long offset = 4GB, long long offset = 17179869184GB = 16777216TB
		uint64_t index_offset_;
		uint64_t resources_offset_;
		uint64_t tmp_offset_;

		SDL_Storage* storage_;
		std::ofstream fs_;
		bool is_ready_ = true;
		std::string label_ = "VisualTextAssetPack;";
		bool is_initialized_ = false;
		std::vector<std::string> img_suffix_list_;
		std::vector<std::string> audio_suffix_list_;
		std::vector<std::string> font_suffix_list_;

		std::map<std::string, uint32_t> reuse_asset_map_;
	public:
		AssetPackStream(const std::string& path, const std::string& dst);
		AssetPackStream& operator<<(const ProgramIndexPushStruct PIPS);
		bool endPackFile();
		size_t getTocTopIndex() const
		{
			return toc_.size() - 1;
		}

		~AssetPackStream();
	};
}

#endif