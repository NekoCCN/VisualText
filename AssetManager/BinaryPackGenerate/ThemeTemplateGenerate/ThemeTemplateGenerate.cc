#include "ThemeTemplateGenerate.h"

bool vtasset::ThemeTemplateGenerator::newGenerator(const std::string& path, const std::string& dst)
{
	return bpga_.operator()(path, dst, ThemeTemplateStr, BinaryPackGenApplication::SuffixModeOff);
}