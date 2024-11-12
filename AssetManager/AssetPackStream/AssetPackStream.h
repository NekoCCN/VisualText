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
	struct AssetStruct  // Change with caution
	{
		uint32_t index;
		uint64_t toc_offset;
		bool is_permanent = false;
		assetformat::AssetFormat asset_format = assetformat::ASSET_FORMAT_FILE;
	};
	struct ProgramIndexPushStruct  // Change with caution
	{
		bool is_permanent = false;
		bool is_init_load = false;
		uint32_t command_argument[3]{};
		std::string asset_filename_list[16]{};
		uint16_t asset_list_index_size = 0;
		assetformat::AssetFormat asset_format_list[16]{};
	};
	struct ProgramIndex
	{
		bool is_permanent = false;
		bool is_init_load = false;
		uint32_t command_argument[3]{};
		uint16_t asset_list_index[16]{};
		uint16_t asset_list_index_size = 0;
		vtcore::command::CommandList command;
	};
	class AssetPackStream
	{
	private:
		std::vector<uint64_t> initialize_loading_resource_index_;
		std::vector<AssetStruct> toc_;  // long offset = 4GB, long long offset = 17179869184GB = 16777216TB
		std::vector<ProgramIndex> program_index_list_;
		SDL_Storage* storage_;
		std::ofstream fs_;
		bool is_ready = false;
		std::string label_ = "VisualTextAssetPack;";
		static bool is_initialized_;
		static std::vector<std::string> img_suffix_list_;
		static std::vector<std::string> audio_suffix_list_;
		static std::vector<std::string> font_suffix_list_;
		uint64_t index_offset_;
		uint64_t resources_offset_;
	public:
		AssetPackStream(std::string path, std::string dst);
		AssetPackStream& operator<<(const ProgramIndexPushStruct PIPS);
		AssetPackStream& pushFile(const std::string file_name, bool is_permanent, bool is_init_load);
		AssetPackStream& endPackFile();
		~AssetPackStream();
	};
}