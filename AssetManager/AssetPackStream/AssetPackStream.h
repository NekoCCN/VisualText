#pragma once
#ifndef VISUALTEXT_ASSETMANAGER_ASSETPACKSTREAM_ASSETPACKSTREAM_H_
#define VISUALTEXT_ASSETMANAGER_ASSETPACKSTREAM_ASSETPACKSTREAM_H_
#include <Core/Exception/Exception.h>
#include <Core/LogSystem/LogSystem.h>
#include <Core/CommandList/CommandList.h>
#include "../AssetFormat/AssetFormat.h"
#include <SDL3/SDL_storage.h>
#include <vector>
#include <cstdint>
#include <cstring>
#include <sstream>

namespace vtasset
{
	struct AssetStruct  // TOC List, Change with caution
	{
		assetformat::AssetFormat asset_format = assetformat::ASSET_FORMAT_FILE;
		uint32_t index;  
		uint64_t toc_offset;
		bool is_permanent = false;
		uint32_t permanent_buffer_index = 0;
	};
	struct ProgramIndexPushStruct  // Program Command List, Change with caution
	{
		vtcore::command::CommandList command;

		bool is_permanent = false;
		bool is_init_load = false;

		bool is_node = false;
		bool is_node_hide = false;
		char node_name[32]{};

		uint32_t command_argument[3]{};

		assetformat::AssetFormat asset_format_list[16]{};
		std::string asset_filename_list[16]{};  // File Mode
		uint16_t asset_list_index_size = 0;

		std::string string;  // String Mode
	};
	struct ProgramIndex
	{
		bool is_permanent = false;
		bool is_init_load = false;

		bool is_node = false;
		bool is_node_hide = false;
		char node_name[32]{};

		uint32_t command_argument[3]{};
		uint16_t asset_list_index[16]{};
		uint16_t asset_list_index_size = 0;
		vtcore::command::CommandList command;
	};
	class AssetPackStream
	{
	private:
		std::vector<ProgramIndex> program_index_list_;
		std::vector<uint64_t> node_list_;
		std::vector<AssetStruct> toc_;  // long offset = 4GB, long long offset = 17179869184GB = 16777216TB
		std::vector<uint64_t> initialize_loading_resource_index_;
		uint64_t index_offset_;
		uint64_t resources_offset_;
		uint64_t tmp_offset_;

		SDL_Storage* storage_;
		std::ofstream fs_;
		bool is_ready = true;
		std::string label_ = "VisualTextAssetPack;";
		bool is_initialized_;
		std::vector<std::string> img_suffix_list_;
		std::vector<std::string> audio_suffix_list_;
		std::vector<std::string> font_suffix_list_;
	public:
		AssetPackStream(std::string path, std::string dst);
		AssetPackStream& operator<<(const ProgramIndexPushStruct PIPS);
		AssetPackStream& endPackFile();
		~AssetPackStream();
	};
}

#endif