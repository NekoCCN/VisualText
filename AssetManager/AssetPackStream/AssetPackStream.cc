#include "AssetPackStream.h"

vtasset::AssetPackStream::AssetPackStream(std::string path, std::string dst)
{
	if (!is_initialized)
	{
		 is_initialized = true;
		 img_suffix_list = 
		 { ".png", ".jpg", ".jpeg", ".bmp", ".avif", ".cur", ".gif", ".ico", ".jxl", ".lbm", ".pnm", ".pcx", ".qol", ".svg", ".tif", ".webp", ".xcf", ".xpm", ".xv" };
		 audio_suffix_list = 
		 { ".mp3", ".wav", ".ogg", ".flac", ".opus" };
		 font_suffix_list = 
		 { ".ttf", ".otf" };
		 vtcore::lst.logIn("Asset pack stream initialized", vtcore::logsys::LOG_PRIORITY_INFO, vtcore::logsys::LOG_CATEGORY_APPLICATION);
	}

	using namespace vtcore;  // Prepare to write file
	SDL_Storage* storage = SDL_OpenFileStorage(path.c_str());
	if (storage == nullptr)
	{
		lst.logIn("Can not open path, did it exist?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		lst.logIn(SDL_GetError(), logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		is_ready = false;
	}
	if (!(SDL_StorageReady(storage)))
	{
		lst.logIn("Can not open path, did it exist?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		lst.logIn(SDL_GetError(), logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		is_ready = false;
	}
	if (SDL_GetPathInfo(dst.c_str(), nullptr))
	{
		lst.logIn("The target file already exists", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		is_ready = false;
	}
	fs_.open(dst, std::ios_base::binary | std::ios_base::app | std::ios_base::out);
	if (!(fs_.is_open()))
	{
		lst.logIn("Can not create binary pack. Path is exist? Have promission?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		is_ready = false;
	}
	fs_.write(label_.c_str(), label_.size() + 1);
	fs_.write((char*)&asset_num_, sizeof(uint32_t));
	fs_.write((char*)&asset_num_, sizeof(uint32_t));
	fs_.write((char*)&initialize_loading_resource_num_, sizeof(uint32_t));
}
vtasset::AssetPackStream::~AssetPackStream()
{
	if (toc_.size() == 0)
		return;

	fs_.seekp(label_.size() + 1, std::ios_base::beg);
	fs_.write((char*)&asset_num_, sizeof(uint32_t));
	fs_.write((char*)&initialize_loading_resource_num_, sizeof(uint32_t));
	AssetStruct* asset_point = new AssetStruct[toc_.size()];
	int i = 0;
	for (AssetStruct& x : toc_)
	{
		asset_point[i] = x;
		++i;
	}
	fs_.seekp(0, std::ios_base::end);
	fs_.write((char*)asset_point, sizeof(AssetStruct) * toc_.size());
	delete[] asset_point;

	uint64_t* initialize_loading_resource_point = new uint64_t[initialize_loading_resource_index_.size()];
	i = 0;
	for (uint64_t& x : initialize_loading_resource_index_)
	{
		initialize_loading_resource_point[i] = x;
		++i;
	}
	fs_.write((char*)initialize_loading_resource_point, sizeof(uint64_t) * initialize_loading_resource_index_.size());
	delete[] initialize_loading_resource_point;
}
vtasset::AssetPackStream& vtasset::AssetPackStream::endPackFile()
{
	if (toc_.size() == 0)
		return;

	fs_.seekp(label_.size() + 1, std::ios_base::beg);
	fs_.write((char*)&asset_num_, sizeof(uint32_t));
	fs_.write((char*)&initialize_loading_resource_num_, sizeof(uint32_t));
	AssetStruct* asset_point = new AssetStruct[toc_.size()];
	int i = 0;
	for (AssetStruct& x : toc_)
	{
		asset_point[i] = x;
		++i;
	}
	fs_.seekp(0, std::ios_base::end);
	fs_.write((char*)asset_point, sizeof(AssetStruct) * toc_.size());
	delete[] asset_point;

	uint64_t* initialize_loading_resource_point = new uint64_t[initialize_loading_resource_index_.size()];
	i = 0;
	for (uint64_t& x : initialize_loading_resource_index_)
	{
		initialize_loading_resource_point[i] = x;
		++i;
	}
	fs_.write((char*)initialize_loading_resource_point, sizeof(uint64_t) * initialize_loading_resource_index_.size());
	delete[] initialize_loading_resource_point;
}
void vtasset::AssetPackStream::getSuffixFormat(AssetPushStruct& APS, std::string suffix)
{
	if (APS.asset_format_list == ASSET_FORMAT_NONE)
	{
		for (auto& x : img_suffix_list)
		{
			if (suffix == x)
			{
				APS.asset_format_list = ASSET_FORMAT_IMAGE;
				break;
			}
		}
	}
	if (APS.asset_format_list == ASSET_FORMAT_NONE)
	{
		for (auto& x : audio_suffix_list)
		{
			if (suffix == x)
			{
				APS.asset_format_list = ASSET_FORMAT_AUDIO;
				break;
			}
		}
	}
	if (APS.asset_format_list == ASSET_FORMAT_NONE)
	{
		for (auto& x : font_suffix_list)
		{
			if (suffix == x)
			{
				APS.asset_format_list = ASSET_FORMAT_FONT;
				break;
			}
		}
	}
}
vtasset::AssetPackStream& vtasset::AssetPackStream::operator<<(const AssetPushStruct APS)
{
	if (APS.asset_format_list == ASSET_FORMAT_NONE)
		throw vtcore::unknown_format_error();

	AssetStruct AS;
	AS.index = toc_.size();  // index = size - 1
	AS.asset_format_list = APS.asset_format_list;
	strncat(AS.asset_label, APS.asset_label_, 24);
	AS.is_permanent = APS.is_permanent;

	uint64_t offset_tmp;
	if (!SDL_GetStoragePathInfo(storage_, APS.file_name.c_str(), nullptr))
	{
		std::ostringstream ost;
		ost << APS.file_name << " in the list does not exist";
		vtcore::lst.logIn(ost.str(), vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_ASSERT);
		throw vtcore::file_not_found_error();
	}
	if (toc_.size() == 0)
	{
		SDL_GetStorageFileSize(storage_, APS.file_name.c_str(), &offset_tmp);
		AS.toc_offset = offset_tmp;
	}
	else
	{
		SDL_GetStorageFileSize(storage_, APS.file_name.c_str(), &offset_tmp);
		AS.toc_offset = offset_tmp + toc_[AS.index - 1].toc_offset;
	}

	++asset_num_;
	toc_.push_back(AS);

	if (APS.is_init_load == true)
	{
		initialize_loading_resource_index_.push_back(toc_[toc_.size() - 1].toc_offset);
		++initialize_loading_resource_num_;
	}
}
vtasset::AssetPackStream& vtasset::AssetPackStream::operator<<(const std::string file_name)
{
	bool get_suffix = false;

	AssetPushStruct APS;
	APS.is_permanent = false;
	APS.is_init_load = false;
	APS.file_name = file_name;
	strncat(APS.asset_label_, file_name.c_str(), 24);

	std::string suffix;
	for (auto& x : file_name)
	{
		if (x == '.')
			get_suffix = true;
		if (get_suffix == true)
			suffix.push_back(x);
	}
	if (get_suffix == false)
		throw vtcore::unknown_format_error();
	
	getSuffixFormat(APS, suffix);

	operator<<(APS);
}
vtasset::AssetPackStream& vtasset::AssetPackStream::pushFile(const std::string file_name, bool is_permanent, bool is_init_load)
{
	bool get_suffix = false;

	AssetPushStruct APS;
	APS.is_permanent = is_permanent;
	APS.is_init_load = is_init_load;
	APS.file_name = file_name;
	strncat(APS.asset_label_, file_name.c_str(), 24);

	std::string suffix;
	for (auto& x : file_name)
	{
		if (x == '.')
			get_suffix = true;
		if (get_suffix == true)
			suffix.push_back(x);
	}
	if (get_suffix == false)
		throw vtcore::unknown_format_error();

	getSuffixFormat(APS, suffix);

	operator<<(APS);
}