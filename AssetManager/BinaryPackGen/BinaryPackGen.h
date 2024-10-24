#pragma once
#ifndef VISUALTEXT_ASSETMANAGER_BINARYPACK_BINARYPACK_H_
#define VISUALTEXT_ASSETMANAGER_BINARYPACKGEN_BINARYPACKGEN_H_
#include <SDL3/SDL.h>
#include <Core/LogSystem/LogSystem.h>
#include <cstdint>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>

class BinaryPackGenApplication
{
private:
	uint64_t* toc_;  // long offset = 4GB, long long offset = 17179869184GB = 16777216TB
	uint32_t file_num_;
	std::vector<std::string> suffix_list_;
	std::string label_ = "VisualTextBinaryPack;";
public:
	enum BigfileMode { BigFileON = 1, BigFileOFF = 0 };  // reserve for future
	bool operator()(std::string path, std::string dst, std::vector<std::string> suffix_list);
	std::vector<std::string>& getSuffixList()
	{
		return suffix_list_;
	}
};
#endif