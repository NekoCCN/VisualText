#include "../BinaryPackGenerate.h"
#include "../../ThemeTemplate/ThemeTemplate.h"
#include <cstdint>


class ThemeTemplateGenerator : protected ThemeTemplate::ThemeAssetList
{
private:
	BinaryPackGenApplication BPGA_;
public:
	bool newGenerator(std::string path, std::string dst);
	bool incrementGenerator(std::string path, std::string dst);
};