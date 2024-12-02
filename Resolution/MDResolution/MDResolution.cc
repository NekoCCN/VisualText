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
		lst.logIn("Can not open md file. Path is exist? Have permission?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_ASSERT);
		is_ready = false;
	}
}
uint64_t vtresolution::MDResolutionApplication::resolutionDefine()
{
	std::string string_buffer;

	uint64_t line = 0;

	uint64_t num_tmp;
	vtcore::command::CommandList command;
	size_t l_tmp, r_tmp;

	fs_.seekg(0, std::ios::beg);

	auto nextLineForDefineResolution = [&]() -> void
		{
			string_buffer.clear();
			std::getline(fs_, string_buffer);
			++line;
		};

	auto errorReset = [&](std::string addition_string = "") -> void
		{
			vtcore::lst.logIn(std::string("Line") + std::to_string(line) + " :" + "Invalid define syntax : " + addition_string,
				vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_APPLICATION);
			fs_.clear();
		};

	nextLineForDefineResolution();
	
	if (fs_.fail())
	{
		errorReset(R"(Unable recognize "---")");
		return false;
	}

	if (string_buffer[0] == '-')
	{
		for (auto& x : string_buffer)
		{
			if (x != '-' || x != '\n')
				errorReset();
				return false;
		}
	}

	while (std::getline(fs_, string_buffer), string_buffer[0] != '-' || string_buffer[0] != EOF || fs_.good())
	{

		string_buffer.clear();

		split_string(string_buffer);

		if ((command = ST.searchString(split_string_buffer[0])) == -1)
		{
			errorReset("Syntax Error");
			return false;
		}

		switch (command)
		{
		case vtcore::command::create_character:
		{
			if (split_string_buffer.size() > 3 || split_string_buffer.size() < 2)
			{
				errorReset("Syntax Error");
				return false;
			}
			if (split_string_buffer[1][0] != '!')
			{
				errorReset("Error declare of asset(image)");
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
					errorReset("Error to find asset(image), Did path right?");
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
					errorReset("Error to find asset(image), Did path right?");
					return false;
				}
				if (speed <= 0)
				{
					errorReset("Speed Error(The Three Argument Error(<0))");
					return false;
				}
				try
				{
					PIPS = CSF_.get_create_character_PIPS_and_create_asset(getPathFromBracket(split_string_buffer[1]), APS_, speed);
				}
				catch (std::exception& e)
				{
					errorReset("Error to find asset(image), Did path right?");
					return false;
				}
			}
			character_define_list_.push_back({ split_string_buffer[1], PIPS });
			break;
		}		
		}

	}

	fs_.clear();

	if (string_buffer[0] == '-')
	{
		for (auto& x : string_buffer)
		{
			if (x != '-' || x != '\n')
				errorReset("Error end statement(---), But still continue");
		}
	}

	return true;
}
vtasset::ProgramIndexPushStruct vtresolution::MDResolutionApplication::resolutionStringToPI(const std::string& str)
{
	std::stringstream ss;
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
		PIPS = CSF_.get_to_main_textbox_PIPS(string_buffer);
		return PIPS;

	case vtcore::command::draw_character:
		ss << string_buffer;
		if (ss.fail())
			throw vtcore::syntax_error_error();
		PIPS = CSF_.get_draw_character_PIPS(string_buffer);
		return PIPS;

	}
}
std::vector<vtasset::ProgramIndexPushStruct> vtresolution::MDResolutionApplication::resolutionCommandStatement(std::string str)
{
	std::vector<int> left_index;
    std::vector<int> right_index;
	std::vector<vtasset::ProgramIndexPushStruct> result;
	for (int i = 0; i < str.size(); ++i)
	{
		if (str[i] == '{')
            left_index.push_back(i);
        if (str[i] == '}')
            right_index.push_back(i);
	}
	if (left_index.size() != right_index.size())
	{
		vtcore::lst.logIn("Line" + std::to_string(line_) + " : " + "Error syntax(Bracket) : " + str, vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_APPLICATION);
		return result;
	}

	for (int i = 0; i < left_index.size(); ++i)
	{
        std::string tmp_str;
        tmp_str = str.substr(left_index[i] + 1, right_index[i] - left_index[i] - 1);
		vtasset::ProgramIndexPushStruct PIPS = resolutionStringToPI(tmp_str);
		result.push_back(PIPS);
	}
	return result;
}
bool vtresolution::MDResolutionApplication::entryPoint()
{
	std::string string_buffer;
	vtasset::ProgramIndexPushStruct PIPS;

	if (!fs_.good())
		return false;
	
	resolutionDefine();

	do
	{
		std::getline(fs_, string_buffer);

		if (string_buffer[0] == '[')
		{
			resolutionNormalStatement(string_buffer);
		}
		else
		{
			std::vector<vtasset::ProgramIndexPushStruct> commandList = resolutionCommandStatement(string_buffer);
			try
			{
				for (auto& x : commandList)
					APS_ << x;
			}
			catch (std::exception& e)
			{
                vtcore::lst.logIn("Line" + std::to_string(line_) + " : " + "Error asset : " + e.what(), vtcore::logsys::LOG_PRIORITY_ERROR, vtcore::logsys::LOG_CATEGORY_APPLICATION);
				return false;
			}
		}
	} while (fs_.good());

	return true;
}