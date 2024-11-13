#include <Core/LogSystem/LogSystem.h>
#include <vector>
#include <cstdint>
#include "../AssetPackStream/AssetPackStream.h"
#include "../MemoryBuffer/MemoryBuffer.h"

namespace vtasset
{
	class AssetPack
	{
	private:
		ProgramIndex* program_index_list_;
		uint64_t* node_list_;
		AssetStruct* toc_;  // long offset = 4GB, long long offset = 17179869184GB = 16777216TB
		uint64_t* initialize_loading_resource_index_;

		uint64_t index_offset_;
		uint64_t resources_offset_;

		uint64_t program_index_num_;
		uint32_t node_num_;
		uint32_t toc_num_;
		uint32_t init_resource_num_;

		std::vector<MemoryBuffer> permanent_buffer_;

		std::ifstream fs_;
		bool is_ready_ = false;
		std::string label_ = "VisualTextAssetPack;";
	public:
		AssetPack(const std::string& path);
		~AssetPack()
		{
			delete[] program_index_list_;
			delete[] node_list_;
			delete[] toc_;
			delete[] initialize_loading_resource_index_;
		}
		uint64_t getFileByte(uint32_t index);
	};
}