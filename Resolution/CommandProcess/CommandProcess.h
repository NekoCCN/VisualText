#include <Core/LogSystem/LogSystem.h>
#include <Core/Exception/Exception.h>
#include <AssetManager/AssetPackStream/AssetPackStream.h>

namespace vtresolution
{
	class CommandStringFactory
	{
	public:
		vtasset::ProgramIndexPushStruct get_create_character_PIPS_and_create_asset(std::string path, vtasset::AssetPackStream& APS, uint32_t speed);
		vtasset::ProgramIndexPushStruct get_to_main_textbox_PIPS(std::string string_data, bool is_node = false, bool is_node_hide = true);
		vtasset::ProgramIndexPushStruct get_draw_character_PIPS(std::string path, bool is_node = false, bool is_node_hide = false);
	};
}