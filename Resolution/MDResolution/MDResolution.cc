#include "MDResolution.h"

void vtresolution::MDResolutionApplication::split_string(const std::string& str)
{
	split_string_buffer.clear();

	std::stringstream ss;
	std::string tmp;

	ss << str;
	ss >> tmp;

	while (ss.fail())
	{
		split_string_buffer.push_back(tmp);
		ss >> tmp;
	}
}
std::string vtresolution::MDResolutionApplication::getPathFromBracket(const std::string& str)
{
	int l_tmp = str.find_first_of('(');
	int r_tmp = str.find_last_of(')');
	if (l_tmp == std::string::npos || r_tmp == std::string::npos)
		return std::string();
	return str.substr(l_tmp + 1, r_tmp - l_tmp - 1);
}
vtresolution::MDResolutionApplication::MDResolutionApplication(const std::string& md_file_path, const std::string& asset_path, const std::string& dst_path)
	: APS_(asset_path, dst_path)
{
	using namespace vtcore;
	if (SDL_GetPathInfo(dst_path.c_str(), nullptr))
	{
		lst.logIn("The target file already exists", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		is_ready = false;
		throw vtcore::file_existed_error();
	}
	fs_.open(dst_path, std::ios_base::in);
	if (!(fs_.is_open()))
	{
		lst.logIn("Can not open md file. Path is exist? Have promission?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		is_ready = false;
	}
}
bool vtresolution::MDResolutionApplication::resolutionDefine()
{
	std::string string_buffer;

	uint64_t num_tmp;
	vtcore::command::CommandList command;
	size_t l_tmp, r_tmp;

	uint64_t offset_tmp = 0;
	offset_tmp = fs_.tellg();

	fs_.seekg(0, std::ios::beg);

	std::getline(fs_, string_buffer);
	
	if (fs_.fail())
	{
		vtcore::lst.logIn("Invalid character define syntax", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_APPLICATION);
		fs_.clear();
		fs_.seekg(offset_tmp, std::ios::beg);
		return false;
	}

	if (string_buffer[0] == '-')
	{
		for (auto& x : string_buffer)
		{
			if (x != '-' || x != '\n')
				return false;
		}
	}

	while (std::getline(fs_, string_buffer), string_buffer[0] != '-' || string_buffer[0] != EOF || fs_.good())
	{

		string_buffer.clear();

		split_string(string_buffer);

		if ((command = ST.searchString(split_string_buffer[0])) == -1)
		{
			vtcore::lst.logIn("Invalid character define syntax", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_APPLICATION);
			fs_.clear();
			fs_.seekg(offset_tmp, std::ios::beg);
			return false;
		}

		switch (command)
		{
		case vtcore::command::create_character:
		{
			if (split_string_buffer.size() > 3 || split_string_buffer.size() < 2)
			{
				vtcore::lst.logIn("Invalid character define syntax", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_APPLICATION);
				fs_.clear();
				fs_.seekg(offset_tmp, std::ios::beg);
				return false;
			}
			if (split_string_buffer[1][0] != '!')
			{
				vtcore::lst.logIn("Invalid character define syntax", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_APPLICATION);
				fs_.seekg(offset_tmp, std::ios::beg);
				fs_.clear();
				return false;
			}

			vtasset::ProgramIndexPushStruct PIPS;

			if (split_string_buffer.size() == 2)
			{
				try
				{
					PIPS = CSF_.get_create_character_PIPS_and_create_asset(getPathFromBracket(split_string_buffer[1]), APS_, 20);
				}
				catch (std::exception& e)
				{
					vtcore::lst.logIn("Invalid character define syntax", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_APPLICATION);
					fs_.seekg(offset_tmp, std::ios::beg);
					fs_.clear();
					return false;
				}
			}
			else
			{
				uint32_t speed;
				try
				{
					speed = std::stoi(split_string_buffer[2]);
				}
				catch (const std::exception& e)
				{
					vtcore::lst.logIn("Invalid character define syntax", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_APPLICATION);
					fs_.clear();
					fs_.seekg(offset_tmp, std::ios::beg);
					return false;
				}
				if (speed <= 0)
				{
					vtcore::lst.logIn("Invalid character define syntax", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_APPLICATION);
					fs_.clear();
					fs_.seekg(offset_tmp, std::ios::beg);
					return false;
				}
				try
				{
					PIPS = CSF_.get_create_character_PIPS_and_create_asset(getPathFromBracket(split_string_buffer[1]), APS_, speed);
				}
				catch (std::exception& e)
				{
					vtcore::lst.logIn("Invalid character define syntax", vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_APPLICATION);
					fs_.seekg(offset_tmp, std::ios::beg);
					fs_.clear();
					return false;
				}
			}
			character_define_list_.push_back({ split_string_buffer[1], PIPS });
			break;
		}		
		}

	}

	fs_.clear();
	fs_.seekg(offset_tmp, std::ios::beg);

	if (string_buffer[0] == '-')
	{
		for (auto& x : string_buffer)
		{
			if (x == '-')
				return true;
		}
	}
}
vtasset::ProgramIndexPushStruct vtresolution::MDResolutionApplication::resolutionStringToPI(const std::string& str)
{
	std::stringstream ss;
	reverse(str.begin(), str.end());
	ss << str;

	std::string string_buffer;
	std::string string_name_buffer;
	vtasset::ProgramIndexPushStruct PIPS;

	ss >> string_buffer;
	vtcore::command::CommandList command = ST.searchString(string_buffer);

	switch (command)
	{
	case vtcore::command::to_main_textbox:
		ss << string_buffer;
		if (ss.fail())
			throw vtcore::syntax_error_error();
		PIPS.command = vtcore::command::to_main_textbox;
		
	}
}