#include "BinaryPackGen.h"

bool BinaryPackGenApplication::operator()(std::string src, std::string dst, std::vector<std::string> suffix_list)
{
	suffix_list_ = std::move(suffix_list);  // Prepare to write file
	SDL_Storage* storage = SDL_OpenFileStorage(src.c_str());
	if (storage == nullptr)
	{
		lst.logIn("Can not open path, did it exist?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		lst.logIn(SDL_GetError(), logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		return false;
	}
	if (!(SDL_StorageReady(storage)))
	{
		lst.logIn("Can not open path, did it exist?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		lst.logIn(SDL_GetError(), logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		return false;
	}
	if (SDL_GetPathInfo(dst.c_str(), nullptr))
	{
		lst.logIn("The target file already exists", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		return false;
	}
	std::ofstream fs;
	fs.open(dst, std::ios_base::binary | std::ios_base::app | std::ios_base::out);
	if (!(fs.is_open()))
	{
		lst.logIn("Can not create binary pack. Path is exist? Have promission?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		return false;
	}

	fs.write(label_.c_str(), label_.size() + 1);

	std::string file_name;  // File Part1 : file num
	uint32_t index = 1;
	file_num_ = 0;
	for (const auto& suffix : suffix_list_)
	{
		index = 1;
		while (SDL_GetStoragePathInfo(storage, (file_name = std::to_string(index++) + "." + suffix).c_str(), nullptr))
		{
			++file_num_;
		}
	}
	fs.write((const char*)&file_num_, sizeof(uint32_t));

	toc_ = new uint64_t[file_num_];  // File Part2 : TOC
	uint32_t toc_index = 0;
	for (const auto& suffix : suffix_list_)
	{
		index = 1;
		while (SDL_GetStoragePathInfo(storage, (file_name = std::to_string(index++) + "." + suffix).c_str(), nullptr))
		{
			SDL_GetStorageFileSize(storage, file_name.c_str(), toc_ + toc_index);
		}
	}
	fs.write((const char*)toc_, file_num_ * sizeof(uint64_t));

	uint64_t size;  // File Part3 : Resources
	char* buffer;
	const uint64_t max_buffer_size = 209715200ll;
	const uint64_t error_buffer_size = 52428800ll;
	for (const auto& suffix : suffix_list_)
	{
		index = 1;
		while (SDL_GetStoragePathInfo(storage, (file_name = std::to_string(index++) + "." + suffix).c_str(), nullptr))
		{
			SDL_GetStorageFileSize(storage, file_name.c_str(), &size);
			if (size >= max_buffer_size)
			{
				try
				{
					buffer = new char[max_buffer_size];
				}
				catch (std::bad_alloc)
				{
					lst.logIn("Memory overflow occurred while creating binary pack. Try increase buffer size", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
					try
					{
						buffer = new char[error_buffer_size];
					}
					catch (std::bad_alloc)
					{
						lst.logIn("Memory overflow occurred while creating binary pack in minimum memory allocation", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
					}
					uint32_t cycle = size / error_buffer_size;
					for (int i = 0; i < cycle; ++i)
					{
						SDL_ReadStorageFile(storage, file_name.c_str(), buffer, error_buffer_size);
						fs.write(buffer, error_buffer_size);
					}
					size %= max_buffer_size;
					SDL_ReadStorageFile(storage, file_name.c_str(), buffer, size);
					fs.write(buffer, size);
					delete[] buffer;
				}
				uint32_t cycle = size / max_buffer_size;
				for (int i = 0; i < cycle; ++i)
				{
					SDL_ReadStorageFile(storage, file_name.c_str(), buffer, max_buffer_size);
					fs.write(buffer, max_buffer_size);
				}
				size %= max_buffer_size;
				SDL_ReadStorageFile(storage, file_name.c_str(), buffer, size);
				fs.write(buffer, size);
				delete[] buffer;
			}
			else
			{
				try
				{
					buffer = new char[size];
				}
				catch (std::bad_alloc)
				{
					lst.logIn("Memory overflow occurred while creating binary pack", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
					return false;
				}
				SDL_ReadStorageFile(storage, file_name.c_str(), buffer, size);
				fs.write(buffer, size);
				delete[] buffer;
			}
		}
	}

	return true;
}