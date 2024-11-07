#include "BinaryPackGenerate.h"


bool vtasset::BinaryPackGenApplication::suffixGen(std::string src, std::string dst, const std::vector<std::string>& suffix_list)
{
	using namespace vtcore;
	list_ = suffix_list;  // Prepare to write file
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
	for (const auto& suffix : list_)
	{
		index = 1;
		while (SDL_GetStoragePathInfo(storage, (file_name = std::to_string(index++) + "." + suffix).c_str(), nullptr))
		{
			++file_num_;
		}
	}
	fs.write((const char*)&file_num_, sizeof(uint32_t));

	toc_ = new uint64_t[file_num_];  // File Part2 : TOC
	uint32_t toc_index = 1;
	toc_[0] = 0;
	uint64_t offset_tmp = 0;
	for (const auto& suffix : list_)
	{
		index = 1;
		while (SDL_GetStoragePathInfo(storage, (file_name = std::to_string(index++) + "." + suffix).c_str(), nullptr))
		{
			SDL_GetStorageFileSize(storage, file_name.c_str(), &offset_tmp);
			if (toc_index == 1)
			{
				toc_[toc_index] = offset_tmp;
			}
			else
			{
				toc_[toc_index] = offset_tmp + toc_[toc_index - 1];
			}
			++toc_index;
		}
	}
	fs.write((char*)toc_, (file_num_ + 1) * sizeof(uint64_t));

	uint64_t size;  // File Part3 : Resources
	char* buffer{};
	const uint64_t max_buffer_size = 209715200ll;
	const uint64_t error_buffer_size = 52428800ll;
	for (const auto& suffix : list_)  // This is a nesting hell. Sooner or later it will have to be rewritten using lambda expression.
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
					lst.logIn(std::string("Include file ") + file_name, logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
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
				lst.logIn(std::string("Include file ") + file_name, logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
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
					lst.logIn(std::string("Include file ") + file_name, logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
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
				lst.logIn(std::string("Include file ") + file_name, logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
				SDL_ReadStorageFile(storage, file_name.c_str(), buffer, size);
				fs.write(buffer, size);
				delete[] buffer;
			}
		}
	}

	fs.close();
	SDL_CloseStorage(storage);

	return true;
}
bool vtasset::BinaryPackGenApplication::fileNameGen(std::string src, std::string dst, const std::vector<std::string>& filename_list)
{
	using namespace vtcore;
	list_ = filename_list;  // Prepare to write file
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

	std::string file_name;  // File Part1 : file check
	file_num_ = list_.size();
	for (const auto& file_name : list_)
	{
		if (!SDL_GetStoragePathInfo(storage, file_name.c_str(), nullptr))
		{
			std::ostringstream ost;
			ost << file_name << " in the list does not exist";
			lst.logIn(ost.str(), logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
			return false;
		}
	}
	fs.write((const char*)&file_num_, sizeof(uint32_t));

	toc_ = new uint64_t[file_num_ + 1];  // File Part2 : TOC
	uint32_t toc_index = 1;
	uint64_t offset_tmp;
	toc_[0] = 0;
	for (const auto& file_name : list_)
	{
		SDL_GetStorageFileSize(storage, file_name.c_str(), &offset_tmp);
		if (toc_index == 1)
		{
			toc_[toc_index] = offset_tmp;
		}
		else
		{
			toc_[toc_index] = offset_tmp + toc_[toc_index - 1];
		}
		++toc_index;
	}
	fs.write((char*)toc_, (file_num_ + 1) * sizeof(uint64_t));

	uint64_t size;  // File Part3 : Resources
	char* buffer{};
	const uint64_t max_buffer_size = 209715200ll;
	const uint64_t error_buffer_size = 52428800ll;
	for (const auto& file_name : list_)  // This is a nesting hell. Sooner or later it will have to be rewritten using lambda expression.
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
					lst.logIn(std::string("Include file ") + file_name, logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
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
			lst.logIn(std::string("Include file ") + file_name, logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
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
				lst.logIn(std::string("Include file ") + file_name, logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
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

			lst.logIn(std::string("Include file ") + file_name, logsys::LOG_PRIORITY_INFO, logsys::LOG_CATEGORY_ASSERT);
			SDL_ReadStorageFile(storage, file_name.c_str(), buffer, size);
			fs.write(buffer, size);
			delete[] buffer;
		}
	}
	fs.close();
	return true;
}
bool vtasset::BinaryPackGenApplication::operator()(std::string path, std::string dst, const std::vector<std::string>& list, SuffixMode smd)  // dont add dot(.)
{
	using namespace vtcore;
	is_suffix_mode_ = (bool)smd;
	if (is_suffix_mode_ == false)
		return fileNameGen(path, dst, list);
	else
		return suffixGen(path, dst, list);
}