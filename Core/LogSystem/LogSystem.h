#pragma once
#ifndef VISUALTEXT_CORE_LOGSYSTEM_LOGSYSTEM_H_
#define VISUALTEXT_CORE_LOGSYSTEM_LOGSYSTEM_H_
#include <string>
#include <SDL3/SDL_log.h>
#include <sstream>
#include <iostream>
#include "../Exception/Exception.h"
#include <ctime>
#include <fstream>
#include <mutex>



namespace logsys
{
	void initLogSys();
	class StandardFilePush
	{
	protected:
		std::mutex mtx_;
		std::ofstream fs_;
		bool iscreate_;
		std::string filename_;
	public:
		StandardFilePush() : iscreate_(false) {  }
		StandardFilePush(bool st);
		StandardFilePush(const char* name);
		StandardFilePush& enableFile(const char* name)
		{
			std::lock_guard<std::mutex> lg(mtx_);
			filename_ = name;
			fs_.open(name);
			if (!(fs_.is_open()))
				throw logsis_init_error();
			iscreate_ = true;
		}
		StandardFilePush& enableFile()
		{
			std::lock_guard<std::mutex> lg(mtx_);
			std::ostringstream ost;
			ost << time(0) << "_Log.log";
			filename_ = ost.str();
			fs_.open(filename_);
			if (!(fs_.is_open()))
				throw logsis_init_error();
			iscreate_ = true;
		}
		virtual ~StandardFilePush()
		{
			if (fs_.is_open())
				fs_.close();
		}
	};
	class FilePush
	{
	private:
		std::mutex m;
		std::ofstream fs;
	public:
		FilePush()
		{
			std::lock_guard<std::mutex> lg(m);
			std::ostringstream ost;
			ost << "log/" << time(0) << "_Log.log";
			fs.open(ost.str());
			if (!(fs.is_open()))
				throw logsis_init_error();
		}
		FilePush& push(std::string str)
		{
			std::lock_guard<std::mutex> lg(m);
			fs << str << std::endl;
			return *this;
		}
	};

	typedef enum LogPriority
	{
		LOG_PRIORITY_INVALID,
		LOG_PRIORITY_TRACE,
		LOG_PRIORITY_VERBOSE,
		LOG_PRIORITY_DEBUG,
		LOG_PRIORITY_INFO,
		LOG_PRIORITY_WARN,
		LOG_PRIORITY_ERROR,
		LOG_PRIORITY_CRITICAL,
		LOG_PRIORITY_COUNT
	} LogPriority;

	typedef enum LogCategory
	{
		LOG_CATEGORY_APPLICATION,
		LOG_CATEGORY_ERROR,
		LOG_CATEGORY_ASSERT,
		LOG_CATEGORY_SYSTEM,
		LOG_CATEGORY_AUDIO,
		LOG_CATEGORY_VIDEO,
		LOG_CATEGORY_RENDER,
		LOG_CATEGORY_INPUT,
		LOG_CATEGORY_TEST,
		LOG_CATEGORY_GPU
	} LogCategory;

	class StandardLogStream : public StandardFilePush
	{
	private:
		char priority_str_[9][10] = 
		{ 
			"[INVALID]",
			"[TRACE]",
			"[VERBOSE]",
			"[DEBUG]",
			"[INFO]",
			"[WARN]", 
			"[ERROR]",
			"[FATAL]",
			"[COUNT]"
		};
		char category_str_[10][9] =
		{
			"[APP]",
			"[ERROR]",
			"[ASSERT]",
			"[SYSTEM]",
			"[AUDIO]",
			"[VIDEO]",
			"[RENDER]",
			"[INPUT]",
			"[TEST]",
			"[GPU]"
		};
	public:
		StandardLogStream() {  }
		StandardLogStream(bool st) : StandardFilePush(st) {  }
		StandardLogStream(const char* name) : StandardFilePush(name) {  }
		StandardLogStream& setType(LogPriority priority = LOG_PRIORITY_INFO, LogCategory category = LOG_CATEGORY_APPLICATION)
		{
			priority_ = priority;
			category_ = category;
			return *this;
		}
		StandardLogStream& operator<<(std::string str);
		StandardLogStream& operator<<(const char* str);
		StandardLogStream& setAllPriority(LogPriority _priority_);
		StandardLogStream& setCategoryPriority(LogCategory category, LogPriority _priority_)
		{
			SDL_SetLogPriority(category, (SDL_LogPriority)_priority_);
			return *this;
		}
		StandardLogStream& logIn(std::string str, LogPriority priority = LOG_PRIORITY_INFO, LogCategory category = LOG_CATEGORY_APPLICATION);
		StandardLogStream& logIn(const char* str, LogPriority priority = LOG_PRIORITY_INFO, LogCategory category = LOG_CATEGORY_APPLICATION);
	private:
		LogPriority priority_ = LOG_PRIORITY_INFO;
		LogCategory category_ = LOG_CATEGORY_APPLICATION;
	};
}

extern logsys::StandardLogStream lst;
#endif