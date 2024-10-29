#include "ThemeTemplateGenerate.h"

bool ThemeTemplateGenerator::newGenerator(std::string path, std::string dst)
{
	return BPGA_.operator()(path, dst, ThemeTemplateStr, BinaryPackGenApplication::SuffixModeOff);
}