#include "AssetPackStream.h"

vtasset::AssetPackStream::AssetPackStream(std::string path, std::string dst) 
	: index_offset_(0), resources_offset_(0), tmp_offset_(0)
{
	if (!is_initialized_)
	{
		 is_initialized_ = true;
		 img_suffix_list_ = 
		 { ".png", ".jpg", ".jpeg", ".bmp", ".avif", ".cur", ".gif", ".ico", ".jxl", ".lbm", ".pnm", ".pcx", ".qol", ".svg", ".tif", ".webp", ".xcf", ".xpm", ".xv" };
		 audio_suffix_list_ = 
		 { ".mp3", ".wav", ".ogg", ".flac", ".opus" };
		 font_suffix_list_ = 
		 { ".ttf", ".otf" };
		 vtcore::lst.logIn("Asset pack stream initialized", vtcore::logsys::LOG_PRIORITY_INFO, vtcore::logsys::LOG_CATEGORY_APPLICATION);
	}

	using namespace vtcore;  // Prepare to write file
	storage_ = SDL_OpenFileStorage(path.c_str());
	if (storage_ == nullptr)
	{
		lst.logIn("Can not open path, did it exist?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		lst.logIn(SDL_GetError(), logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		is_ready = false;
	}
	if (!(SDL_StorageReady(storage_)))
	{
		lst.logIn("Can not open path, did it exist?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		lst.logIn(SDL_GetError(), logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		is_ready = false;
	}
	if (SDL_GetPathInfo(dst.c_str(), nullptr))
	{
		lst.logIn("The target file already exists", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		is_ready = false;
		throw vtcore::file_existed_error();
	}
	fs_.open(dst, std::ios_base::binary | std::ios_base::out);
	if (!(fs_.is_open()))
	{
		lst.logIn("Can not create binary pack. Path is exist? Have permission?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		is_ready = false;
	}

	resources_offset_ = sizeof(uint64_t) * 3 + 3 * sizeof(uint32_t) + label_.size() + 1;

	fs_.write(label_.c_str(), label_.size() + 1);
	fs_.write((char*)&resources_offset_, sizeof(uint64_t));  // resources offset
	fs_.write((char*)&index_offset_, sizeof(uint64_t));  // index offset
	uint64_t tmp1 = 0;
	uint32_t tmp2 = 0;
	fs_.write((char*)&tmp1, sizeof(uint64_t));  // Program_Index num
	fs_.write((char*)&tmp2, sizeof(uint32_t));  // Node num
	fs_.write((char*)&tmp2, sizeof(uint32_t));  // TOC num
	fs_.write((char*)&tmp2, sizeof(uint32_t));  // Init_resource num
}
vtasset::AssetPackStream::~AssetPackStream()
{
	if (toc_.size() == 0)
		return;

    // Write leading data
	fs_.seekp(0, std::ios_base::end);  // index offset
	index_offset_ = fs_.tellp();

	fs_.seekp(label_.size() + 1 + sizeof(uint64_t), std::ios_base::beg);

	fs_.write((char*)&index_offset_, sizeof(uint64_t));

	uint64_t tmp1 = program_index_list_.size();
	fs_.write((char*)&tmp1, sizeof(uint64_t));  // Program_Index num

	uint32_t tmp2 = node_list_.size();
	fs_.write((char*)&tmp2, sizeof(uint32_t));  // Node num

	tmp2 = toc_.size();
	fs_.write((char*)&tmp2, sizeof(uint32_t));  // TOC num
	tmp2 = initialize_loading_resource_index_.size();
	fs_.write((char*)&tmp2, sizeof(uint32_t));  // Init_resource num

	fs_.seekp(0, std::ios_base::end);
	
	fs_.write((char*)program_index_list_.data(), sizeof(ProgramIndex) * program_index_list_.size()); // Write program index
	fs_.write((char*)node_list_.data(), sizeof(uint64_t) * initialize_loading_resource_index_.size());  // Write Node list
	fs_.write((char*)toc_.data(), sizeof(AssetStruct) * toc_.size());  // Write Toc
	fs_.write((char*)initialize_loading_resource_index_.data(), sizeof(uint64_t) * initialize_loading_resource_index_.size());  // Write initialize loading resource`s index

	fs_.close();
}
bool vtasset::AssetPackStream::endPackFile()
{
	if (toc_.size() == 0)
		return false;

	// Write leading data
	fs_.seekp(0, std::ios_base::end);  // index offset
	index_offset_ = fs_.tellp();

	fs_.seekp(label_.size() + 1 + sizeof(uint64_t), std::ios_base::beg);

	fs_.write((char*)&index_offset_, sizeof(uint64_t));

	uint64_t tmp1 = program_index_list_.size();
	fs_.write((char*)&tmp1, sizeof(uint64_t));  // Program_Index num

	uint32_t tmp2 = node_list_.size();
	fs_.write((char*)&tmp2, sizeof(uint32_t));  // Node num

	tmp2 = toc_.size();
	fs_.write((char*)&tmp2, sizeof(uint32_t));  // TOC num
	tmp2 = initialize_loading_resource_index_.size();
	fs_.write((char*)&tmp2, sizeof(uint32_t));  // Init_resource num

	fs_.seekp(0, std::ios_base::end);

	fs_.write((char*)program_index_list_.data(), sizeof(ProgramIndex) * program_index_list_.size()); // Write program index
	fs_.write((char*)node_list_.data(), sizeof(uint64_t) * initialize_loading_resource_index_.size());  // Write Node list
	fs_.write((char*)toc_.data(), sizeof(AssetStruct) * toc_.size());  // Write Toc
	fs_.write((char*)initialize_loading_resource_index_.data(), sizeof(uint64_t) * initialize_loading_resource_index_.size());  // Write initialize loading resource`s index

	fs_.close();
	return true;
}
vtasset::AssetPackStream& vtasset::AssetPackStream::operator<<(const ProgramIndexPushStruct PIPS)
{
	if (is_ready == false)
		return *this;
	using namespace vtcore;

	ProgramIndex PI;

	if (PIPS.reuse_asset == false)
	{
		AssetStruct* ASList = new AssetStruct[PIPS.asset_list_index_size];
		for (int i = 0; i < PIPS.asset_list_index_size; ++i)
		{
			ASList[i].index = toc_.size();
			ASList[i].is_permanent = PIPS.is_permanent;
			ASList[i].asset_format = PIPS.asset_format_list[i];
			if (ASList[i].is_permanent == true)
			{
				ASList[i].permanent_buffer_index = initialize_loading_resource_index_.size();
				initialize_loading_resource_index_.push_back(ASList[i].index);
			}
			if (ASList[i].asset_format == assetformat::ASSET_FORMAT_FILE)
			{
				if (!SDL_GetStoragePathInfo(storage_, PIPS.asset_filename_list[i].c_str(), nullptr))
				{
					std::ostringstream ost;
					ost << PIPS.asset_filename_list[i].c_str() << " in the list does not exist";
					vtcore::lst.logIn(ost.str(), vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_ASSERT);
					throw vtcore::file_not_found_error();
				}

				if (toc_.size() == 0)
				{
					ASList[i].toc_offset = 0;
					SDL_GetStorageFileSize(storage_, PIPS.asset_filename_list[i].c_str(), &tmp_offset_);
				}
				else
				{
					ASList[i].toc_offset = toc_[toc_.size() - 1].toc_offset + tmp_offset_;
					SDL_GetStorageFileSize(storage_, PIPS.asset_filename_list[i].c_str(), &tmp_offset_);
				}

				toc_.push_back(ASList[i]);

				// Start write resource
				uint64_t size;
				char* buffer{};
				const uint64_t max_buffer_size = 209715200ll;
				const uint64_t error_buffer_size = 52428800ll;
				bool flag = false;
				SDL_GetStorageFileSize(storage_, PIPS.asset_filename_list[i].c_str(), &size);
				if (size >= max_buffer_size)
				{
					try
					{
						buffer = new char[max_buffer_size];
					}
					catch (std::bad_alloc)
					{
						flag = true;
						vtcore::lst.logIn("Memory overflow occurred while creating binary pack. Try increase buffer size", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
						try
						{
							buffer = new char[error_buffer_size];
						}
						catch (std::bad_alloc)
						{
							lst.logIn("Memory overflow occurred while creating binary pack in minimum memory allocation", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
							throw out_of_memory();
						}
						uint32_t cycle = size / error_buffer_size;
						lst.logIn(std::string("Include file ") + PIPS.asset_filename_list[i].c_str(), logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
						for (int i = 0; i < cycle; ++i)
						{
							SDL_ReadStorageFile(storage_, PIPS.asset_filename_list[i].c_str(), buffer, error_buffer_size);
							fs_.write(buffer, error_buffer_size);
						}
						size %= max_buffer_size;
						SDL_ReadStorageFile(storage_, PIPS.asset_filename_list[i].c_str(), buffer, size);
						fs_.write(buffer, size);
						delete[] buffer;
					}
					if (flag == false)
					{
						uint32_t cycle = size / max_buffer_size;
						lst.logIn(std::string("Include file ") + PIPS.asset_filename_list[i], logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
						for (int i = 0; i < cycle; ++i)
						{
							SDL_ReadStorageFile(storage_, PIPS.asset_filename_list[i].c_str(), buffer, max_buffer_size);
							fs_.write(buffer, max_buffer_size);
						}
						size %= max_buffer_size;
						SDL_ReadStorageFile(storage_, PIPS.asset_filename_list[i].c_str(), buffer, size);
						fs_.write(buffer, size);
						delete[] buffer;
					}
					else flag = false;
				}
				else
				{
					try
					{
						buffer = new char[size];
					}
					catch (std::bad_alloc)
					{
						flag = true;
						lst.logIn("Memory overflow occurred while creating binary pack. Try increase buffer size", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
						try
						{
							buffer = new char[error_buffer_size];
						}
						catch (std::bad_alloc)
						{
							lst.logIn("Memory overflow occurred while creating binary pack in minimum memory allocation", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
							throw out_of_memory();
						}
						uint32_t cycle = size / error_buffer_size;
						lst.logIn(std::string("Include file ") + PIPS.asset_filename_list[i], logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
						for (int i = 0; i < cycle; ++i)
						{
							SDL_ReadStorageFile(storage_, PIPS.asset_filename_list[i].c_str(), buffer, error_buffer_size);
							fs_.write(buffer, error_buffer_size);
						}
						size %= max_buffer_size;
						SDL_ReadStorageFile(storage_, PIPS.asset_filename_list[i].c_str(), buffer, size);
						fs_.write(buffer, size);
						delete[] buffer;
					}
					if (flag == false)
					{
						lst.logIn(std::string("Include file ") + PIPS.asset_filename_list[i], logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
						SDL_ReadStorageFile(storage_, PIPS.asset_filename_list[i].c_str(), buffer, size);
						fs_.write(buffer, size);
						delete[] buffer;
					}
					else flag = false;
				}
			}
			if (ASList[i].asset_format == assetformat::ASSET_FORMAT_STRING)
			{
				if (toc_.size() == 0)
				{
					ASList[i].toc_offset = 0;
					tmp_offset_ = PIPS.string.size() + 1;
				}
				else
				{
					ASList[i].toc_offset = toc_[toc_.size() - 1].toc_offset + tmp_offset_;
					tmp_offset_ = PIPS.string.size() + 1;
				}
				toc_.push_back(ASList[i]);
				ASList[i].is_permanent = false;
				lst.logIn(std::string("Include string ") + PIPS.string.substr(0, 10), logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_APPLICATION);
				fs_.write(PIPS.string.c_str(), PIPS.string.size() + 1);
			}
		}
		for (int i = 0; i < PIPS.asset_list_index_size; ++i)
		{
			PI.asset_list_index[i] = ASList[i].index;
		}
	}

	if (PIPS.reuse_asset == true)
	{
		memcpy(PI.asset_list_index, PIPS.reuse_asset_index, 10 * sizeof(uint32_t));
	}
	PI.asset_list_index_size = PIPS.asset_list_index_size;
	PI.command = PIPS.command;
	for (int i = 0; i < 3; ++i)
		PI.command_argument[i] = PIPS.command_argument[i];
	PI.is_init_load = PIPS.is_init_load;
	PI.is_node = PIPS.is_node;
	PI.is_node_hide = PIPS.is_node_hide;
	PI.is_permanent = PIPS.is_permanent;
	memcpy(PI.node_name, PIPS.node_name, 32);
	program_index_list_.push_back(PI);

	if (PI.is_node == true)
		node_list_.push_back(program_index_list_.size() - 1);
	return *this;
}