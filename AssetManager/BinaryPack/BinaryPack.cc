#include "BinaryPack.h"

vtasset::BinaryPack::BinaryPack(std::string path)
{
	using namespace vtcore;
	fs_.open(path, std::ios_base::in | std::ios_base::binary);
	if (!(fs_.is_open()))
	{
		lst.logIn("Path is not exist", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		throw path_not_exists();
	}
	char* str = new char[label_offset];
	char label[22] = "VisualTextBinaryPack;";
	fs_.read(str, label_offset);
	for (uint32_t i = 0; i < 22; ++i)
	{
		if (str[i] != label[i])
		{
			lst.logIn("Not a Visual Text Pack file", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
			throw invalid_file_format();
		}
	}
	delete[] str;

	is_ready_ = true;
	fs_.read((char*)&file_num_, sizeof(uint32_t));
	toc_ = new uint64_t[file_num_ + 1];
	fs_.read((char*)toc_, sizeof(uint64_t) * (file_num_ + 1));
	resource_offset_ = sizeof(uint64_t) * (file_num_ + 1) + sizeof(uint32_t) + label_offset;
}
vtasset::BinaryPack::BinaryPack(std::ifstream& fs)
{
	using namespace vtcore;
	fs_ = std::move(fs);
	if (fs_.is_open())
	{
		lst.logIn("Path is not exist", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		throw path_not_exists();
	}
	char* str = new char[label_offset];
	char label[22] = "VisualTextBinaryPack;";
	fs_.read(str, label_offset);
	for (uint32_t i = 0; i < 22; ++i)
	{
		if (str[i] != label[i])
		{
			lst.logIn("Not a Visual Text Pack file", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
			throw invalid_file_format();
		}
	}
	is_ready_ = true;
	fs_.read((char*)&file_num_, sizeof(uint32_t));
	toc_ = new uint64_t[file_num_ + 1];
	fs_.read((char*)toc_, sizeof(uint64_t) * (file_num_ + 1));
	resource_offset_ = sizeof(uint64_t) * (file_num_ + 1) + sizeof(uint32_t) + label_offset;
}
bool vtasset::BinaryPack::loadToBuffer_s(uint32_t index, char* buffer, uint64_t buffer_size)
{
	using namespace vtcore;
	if (buffer_size < toc_[index])
		return 0;
	if (file_num_ < index + 1)
		return 0;
	uint64_t offset = toc_[index];
	fs_.seekg(offset + resource_offset_, std::ios_base::beg);
	fs_.read(buffer, getBufferSize(index));
	return 1;
}
bool vtasset::BinaryPack::loadToBuffer(uint32_t index, char* buffer)
{
	using namespace vtcore;
	if (file_num_ < index + 1)
		return 0;
	uint64_t offset = toc_[index];
	fs_.seekg(offset + resource_offset_, std::ios_base::beg);
	fs_.read(buffer, toc_[index]);
	return 1;
}
uint32_t vtasset::BinaryPack::getFileNum(uint32_t index)
{
	return file_num_;
}
uint64_t vtasset::BinaryPack::getBufferSize(uint32_t index)
{
	return toc_[index + 1] - toc_[index];
}
char* vtasset::BinaryPack::operator[](uint32_t index)
{
	if (file_num_ < index + 1)
		return nullptr;
	char* tmp = new char[getBufferSize(index)];
	fs_.seekg(toc_[index] + resource_offset_, std::ios_base::beg);
	fs_.read(tmp, getBufferSize(index));
	return tmp;
}
bool vtasset::BinaryPack::open(std::string path)
{
	using namespace vtcore;
	fs_.open(path, std::ios_base::in | std::ios_base::binary);
	if (fs_.is_open())
	{
		lst.logIn("Path is not exist", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		return false;
	}
	char* str = new char[label_offset];
	char label[22] = "VisualTextBinaryPack;";
	fs_.read(str, label_offset);
	for (uint32_t i = 0; i < 22; ++i)
	{
		if (str[i] != label[i])
		{
			lst.logIn("Not a Visual Text Pack file", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
			return false;
		}
	}
	is_ready_ = true;
	resource_offset_ = sizeof(uint64_t) * file_num_ + sizeof(uint32_t) + label_offset;
	fs_.read((char*)&file_num_, sizeof(uint32_t));
	toc_ = new uint64_t[file_num_ + 1];
	fs_.read((char*)toc_, sizeof(uint64_t) * (file_num_ + 1));
	resource_offset_ = sizeof(uint64_t) * (file_num_ + 1) + sizeof(uint32_t) + label_offset;
}
bool vtasset::BinaryPack::open(std::ifstream& fs)
{
	using namespace vtcore;
	fs_ = std::move(fs);
	if (fs_.is_open())
	{
		lst.logIn("Path is not exist", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		return false;
	}
	char* str = new char[label_offset];
	char label[22] = "VisualTextBinaryPack;";
	fs_.read(str, label_offset);
	for (uint32_t i = 0; i < 22; ++i)
	{
		if (str[i] != label[i])
		{
			lst.logIn("Not a Visual Text Pack file", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
			return false;
		}
	}
	is_ready_ = true;
	resource_offset_ = sizeof(uint64_t) * file_num_ + sizeof(uint32_t) + label_offset;
	fs_.read((char*)&file_num_, sizeof(uint32_t));
	toc_ = new uint64_t[file_num_ + 1];
	fs_.read((char*)toc_, sizeof(uint64_t) * (file_num_ + 1));
	resource_offset_ = sizeof(uint64_t) * (file_num_ + 1) + sizeof(uint32_t) + label_offset;
}