#pragma once
#ifndef VT_RESOLUTION_MDRESOLUTION_MDRESOLUTION_H
#define VT_RESOLUTION_MDRESOLUTION_MDRESOLUTION_H

#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <cstdint>
#include <Core/CommandList/CommandList.h>
#include <Core/Exception/Exception.h>
#include <Core/LogSystem/LogSystem.h>
#include <AssetManager/AssetPackStream/AssetPackStream.h>
#include <AssetManager/AssetPack/AssetPack.h>
#include "../CommandProcess/CommandProcess.h"
#include "../SemanticTable/SemanticTable.h"

namespace vtresolution
{
	class MDResolutionApplication
	{
	private:
		CommandStringFactory CSF_;

		vtasset::AssetPackStream APS_;
		std::ifstream fs_;
		bool is_ready;
		typedef std::pair<std::string, vtasset::ProgramIndexPushStruct> DefinePair;
		std::vector<DefinePair> command_define_list_;
		std::vector<DefinePair> character_define_list_;
		SemanticTable ST;
		uint64_t line = 0;

		std::vector<std::string> split_string_buffer;
		void split_string(const std::string& str);
		std::string getPathFromBracket(const std::string& str);

		typedef std::pair<std::string, uint32_t> filename_to_toc_index_;

		
	public:
		MDResolutionApplication(const std::string& md_file_path, const std::string& asset_path, const std::string& dst_path);
		bool resolutionDefine();
		vtasset::ProgramIndexPushStruct resolutionStringToPI(const std::string& str);  // throw a exception
	};
}

#endif