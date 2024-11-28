#include <Core/LogSystem/LogSystem.h>
#include <Core/Exception/Exception.h>
#include <AssetManager/AssetPackStream/AssetPackStream.h>

namespace vtresolution
{
	class CommandStringFactory
	{
	public:
		vtasset::ProgramIndexPushStruct get_create_character_PIPS_and_create_asset(std::string path, vtasset::AssetPackStream& APS, uint32_t speed);
	};
}