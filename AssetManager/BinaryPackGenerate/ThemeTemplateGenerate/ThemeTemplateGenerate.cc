#include "ThemeTemplateGenerate.h"

bool vtasset::ThemeTemplateGenerator::newGenerator(std::string path, std::string dst)
{
	return BPGA_.operator()(path, dst, ThemeTemplateStr, BinaryPackGenApplication::SuffixModeOff);
}