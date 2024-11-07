#include <Core/Exception/Exception.h>
#include <Core/LogSystem/LogSystem.h>
#include <SDL3/SDL_storage.h>
#include <vector>
#include <cstdint>
#include <cstring>

namespace vtasset
{
	enum AssetFormatList
	{
		ASSET_FORMAT_IMAGE,
		ASSET_FORMAT_AUDIO,
		ASSET_FORMAT_COMMEND,
		ASSET_FORMAT_FONT,
		ASSET_FORMAT_NONE
	};
	struct AssetStruct  // Change with caution
	{
		uint32_t index;
		uint64_t toc_offset;
		char asset_label[24] = "NULL";
		bool is_permanent = false;
		AssetFormatList asset_format_list = ASSET_FORMAT_NONE;
	};
	struct AssetPushStruct  // Change with caution
	{
		std::string file_name;
		char asset_label_[24] = "NULL";
		bool is_permanent = false;
		bool is_init_load = false;
		AssetFormatList asset_format_list = ASSET_FORMAT_NONE;
	};
	class AssetPackStream
	{
	private:
		std::vector<uint64_t> initialize_loading_resource_index_;
		std::vector<AssetStruct> toc_;  // long offset = 4GB, long long offset = 17179869184GB = 16777216TB
		uint32_t asset_num_;
		uint32_t initialize_loading_resource_num_;
		std::vector<std::string> list_;
		SDL_Storage* storage_;
		std::ofstream fs_;
		bool is_ready = false;
		std::string label_ = "VisualTextAssetPack;";
		static bool is_initialized;
		static std::vector<std::string> img_suffix_list;
		static std::vector<std::string> audio_suffix_list;
		static std::vector<std::string> font_suffix_list;
		void getSuffixFormat(AssetPushStruct& APS, std::string suffix);
	public:
		AssetPackStream(std::string path, std::string dst);
		AssetPackStream& operator<<(const std::string file_name);
		AssetPackStream& operator<<(const AssetPushStruct APS);
		AssetPackStream& pushFile(const std::string file_name, bool is_permanent, bool is_init_load);
		AssetPackStream& endPackFile();
		~AssetPackStream();
	};
}