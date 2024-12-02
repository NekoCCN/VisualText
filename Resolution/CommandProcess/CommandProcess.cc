#include "CommandProcess.h"

vtasset::ProgramIndexPushStruct vtresolution::CommandStringFactory::get_create_character_PIPS_and_create_asset(std::string path, vtasset::AssetPackStream& APS, uint32_t speed)
{
	vtasset::ProgramIndexPushStruct PIPS1;
	PIPS1.command = vtcore::command::only_asset;
	PIPS1.asset_format_list[0] = vtasset::assetformat::ASSET_FORMAT_FILE;
	PIPS1.asset_list_index_size = 1;
	PIPS1.asset_filename_list[0] = path;
	APS << PIPS1;
	PIPS1.command = vtcore::command::draw_character;
	return PIPS1;
}
vtasset::ProgramIndexPushStruct vtresolution::CommandStringFactory::get_to_main_textbox_PIPS(std::string string_data, bool is_node, bool is_node_hide)
{
    vtasset::ProgramIndexPushStruct PIPS;
	PIPS.command = vtcore::command::to_main_textbox;
	PIPS.asset_format_list[0] = vtasset::assetformat::ASSET_FORMAT_STRING;
	PIPS.asset_list_index_size = 1;
	PIPS.string = string_data;
	PIPS.is_node = is_node;
    PIPS.is_node_hide = is_node_hide;
    return PIPS;
}
vtasset::ProgramIndexPushStruct vtresolution::CommandStringFactory::get_draw_character_PIPS(std::string path, bool is_node, bool is_node_hide)
{
	vtasset::ProgramIndexPushStruct PIPS;
	PIPS.command = vtcore::command::draw_character;
	PIPS.asset_format_list[0] = vtasset::assetformat::ASSET_FORMAT_FILE;
	PIPS.asset_list_index_size = 1;
	PIPS.asset_filename_list[0] = path;
	PIPS.is_node = is_node;
	PIPS.is_node_hide = is_node_hide;
	return PIPS;
}