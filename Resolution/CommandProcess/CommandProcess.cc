#include "CommandProcess.h"

#include <utility>

vtasset::ProgramIndexPushStruct vtresolution::CommandStringFactory::get_create_character_PIPS_and_create_asset(std::string path, vtasset::AssetPackStream& APS, uint32_t speed)
{
	vtasset::ProgramIndexPushStruct pips1;
	pips1.command = vtcore::command::only_asset;
	pips1.asset_format_list[0] = vtasset::assetformat::ASSET_FORMAT_FILE;
	pips1.asset_list_index_size = 1;
	pips1.asset_filename_list[0] = std::move(path);
	APS << pips1;
	pips1.command = vtcore::command::draw_character;
	return pips1;
}
vtasset::ProgramIndexPushStruct vtresolution::CommandStringFactory::get_to_main_textbox_PIPS(const std::string&
	string_data, bool is_node, bool is_node_hide)
{
    vtasset::ProgramIndexPushStruct pips;
	pips.command = vtcore::command::to_main_textbox;
	pips.asset_format_list[0] = vtasset::assetformat::ASSET_FORMAT_STRING;
	pips.asset_list_index_size = 1;
	pips.string = std::move(string_data);
    pips.is_node = is_node;
    pips.is_node_hide = is_node_hide;
    return pips;
}
vtasset::ProgramIndexPushStruct vtresolution::CommandStringFactory::get_draw_character_PIPS(const std::string& path, bool is_node, bool is_node_hide)
{
	vtasset::ProgramIndexPushStruct pips;
	pips.command = vtcore::command::draw_character;
	pips.asset_format_list[0] = vtasset::assetformat::ASSET_FORMAT_FILE;
	pips.asset_list_index_size = 1;
	pips.asset_filename_list[0] = std::move(path);
	pips.is_node = is_node;
	pips.is_node_hide = is_node_hide;
	return pips;
}