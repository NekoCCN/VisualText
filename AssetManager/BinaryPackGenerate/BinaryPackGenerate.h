#pragma once
#ifndef VISUALTEXT_ASSETMANAGER_BINARYPACKGEN_BINARYPACKGEN_H_
#define VISUALTEXT_ASSETMANAGER_BINARYPACKGEN_BINARYPACKGEN_H_
#include <SDL3/SDL.h>
#include <Core/LogSystem/LogSystem.h>
#include <cstdint>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>

namespace vtasset
{
	class BinaryPackGenApplication
	{
	private:
		uint64_t* toc_ = 0;  // long offset = 4GB, long long offset = 17179869184GB = 16777216TB
		uint32_t file_num_ = 0;
		std::vector<std::string> list_;
		std::string label_ = "VisualTextBinaryPack;";
		bool is_suffix_mode_ = false;
	public:
		enum BigfileMode { BigFileON = 1, BigFileOFF = 0 };  // reserve for future
		enum SuffixMode { SuffixModeOn = 1, SuffixModeOff = 0 };
		bool operator()(std::string path, std::string dst, const std::vector<std::string>& list, SuffixMode smd = SuffixModeOff);
		std::vector<std::string>& getList()
		{
			return list_;
		}
	private:
		bool suffixGen(std::string src, std::string dst, const std::vector<std::string>& suffix_list);
		bool fileNameGen(std::string src, std::string dst, const std::vector<std::string>& filename_list);
	};
}
#endif