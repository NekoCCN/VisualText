#include "AssetPack.h"

vtasset::AssetPack::AssetPack(const std::string& path)
{
	using namespace vtcore;
	fs_.open(path, std::ios_base::in | std::ios_base::binary);
	if (!(fs_.is_open()))
	{
		lst.logIn("Path is not exist", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		throw path_not_exists();
	}
	char* str = new char[label_.size() + 1];
	fs_.read(str, label_.size() + 1);
	for (uint32_t i = 0; i < label_.size(); ++i)
	{
		if (str[i] != label_[i])
		{
			lst.logIn("Not a Visual Text Pack file", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
			throw invalid_file_format();
		}
	}
	is_ready_ = true;

	fs_.read((char*)&resources_offset_, sizeof(uint64_t));  // resources offset
	fs_.read((char*)&index_offset_, sizeof(uint64_t));  // index offset
	fs_.read((char*)&program_index_num_, sizeof(uint64_t));  // Program_Index num
	fs_.read((char*)&node_num_, sizeof(uint32_t));  // Node num
	fs_.read((char*)&toc_num_, sizeof(uint32_t));  // TOC num
	fs_.read((char*)&init_resource_num_, sizeof(uint32_t));  // Init_resource num

	fs_.seekg(index_offset_, std::ios_base::beg);

	program_index_list_ = new ProgramIndex[program_index_num_];
	node_list_ = new uint64_t[node_num_];
	toc_ = new AssetStruct[toc_num_];
	initialize_loading_resource_index_ = new uint64_t[init_resource_num_];

	fs_.read((char*)program_index_list_, sizeof(ProgramIndex) * program_index_num_);
	fs_.read((char*)node_list_, sizeof(uint64_t) * node_num_);
	fs_.read((char*)toc_, sizeof(AssetStruct) * toc_num_);
	fs_.read((char*)initialize_loading_resource_index_, sizeof(uint64_t) * init_resource_num_);

	char* tmp;
	for (uint32_t i = 0; i < init_resource_num_; i++)
	{
		if (toc_[initialize_loading_resource_index_[i]].is_permanent == true)
		{
			tmp = new char[getFileByte(i)];
			fs_.read(tmp, getFileByte(i));
			permanent_buffer_.push_back({ tmp, getFileByte(i) });
		}
	}

	program_index_pointer_ = program_index_list_;
}
uint64_t vtasset::AssetPack::getFileByte(uint32_t index)
{
	if (index != toc_num_ - 1)
		return toc_[index + 1].toc_offset - toc_[index].toc_offset;
	else
		return index_offset_ - toc_[index].toc_offset;
}
vtasset::AssetPack& vtasset::AssetPack::operator>>(ProgramIndex& PI)
{
	++program_index_pointer_;
	PI = *(program_index_pointer_ - 1);
	return *this;
}
bool vtasset::AssetPack::goProgramIndex(uint32_t index)
{
	if (index >= program_index_num_ - 1 || index < 0)
		return false;
    program_index_pointer_ = program_index_list_ + index;
	return true;
}
bool vtasset::AssetPack::getMemoryBuffer(uint32_t index, MemoryBuffer& MBuffer)
{
	if (index > toc_num_ - 1 || index < 0)
		return false;
	if (MBuffer.isEmpty() != true)
		return false;
	char* tmp = new char[getFileByte(index)];
	fs_.seekg(resources_offset_, std::ios_base::beg);
	fs_.read(tmp, getFileByte(index));

	MBuffer.buffer_ = tmp;
	MBuffer.byte_size_ = getFileByte(index);

	return true;
}