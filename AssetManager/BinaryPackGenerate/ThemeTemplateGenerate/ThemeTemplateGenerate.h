#include "../BinaryPackGenerate.h"
#include "../../ThemeTemplate/ThemeTemplate.h"
#include <cstdint>


namespace vtasset
{
	class ThemeTemplateGenerator : protected ThemeAssetList
	{
	private:
		BinaryPackGenApplication bpga_;
	public:
		bool newGenerator(const std::string& path, const std::string& dst);
		bool incrementGenerator(std::string path, std::string dst);
	};
}