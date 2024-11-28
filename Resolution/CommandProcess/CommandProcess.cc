#include "CommandProcess.h"

vtasset::ProgramIndexPushStruct vtresolution::CommandStringFactory::get_create_character_PIPS_and_create_asset(std::string path, vtasset::AssetPackStream& APS, uint32_t speed)
{
	vtasset::ProgramIndexPushStruct PIPS1;
	PIPS1.command = vtcore::command::only_asset;
	PIPS1.reuse_asset = false;
	PIPS1.asset_format_list[0] = vtasset::assetformat::ASSET_FORMAT_FILE;
	PIPS1.asset_list_index_size = 1;
	PIPS1.asset_filename_list[0] = path;
	APS << PIPS1;
	vtasset::ProgramIndexPushStruct PIPS2;
	PIPS2.command = vtcore::command::draw_character;
	PIPS2.reuse_asset = true;
	PIPS2.reuse_asset_index[0] = APS.getTocTopIndex();
	PIPS2.asset_format_list[0] = vtasset::assetformat::ASSET_FORMAT_FILE;
	PIPS2.asset_list_index_size = 1;
	PIPS2.command_argument[0] = speed;
	return PIPS2;
}