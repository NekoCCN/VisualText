#include "LogSystem.h"

using namespace logsys;

void initLogSys()
{
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_COUNT, "[COUNT]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_CRITICAL, "[FATAL]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_DEBUG, "[DEBUG]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_ERROR, "[ERROR]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_INFO, "[INFO]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_INVALID, "[INVALID]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_TRACE, "[TRACE]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_VERBOSE, "[VERBOSE]");
	SDL_SetLogPriorityPrefix(SDL_LOG_PRIORITY_WARN, "[WARN]");
}
StandardLogStream& StandardLogStream::setAllPriority(LogPriority _priority_)
{
	SDL_LogPriority priority = (SDL_LogPriority)_priority_;
	SDL_SetLogPriority(LOG_CATEGORY_APPLICATION, priority);
	SDL_SetLogPriority(LOG_CATEGORY_ASSERT, priority);
	SDL_SetLogPriority(LOG_CATEGORY_AUDIO, priority);
	SDL_SetLogPriority(LOG_CATEGORY_ERROR, priority);
	SDL_SetLogPriority(LOG_CATEGORY_GPU, priority);
	SDL_SetLogPriority(LOG_CATEGORY_INPUT, priority);
	SDL_SetLogPriority(LOG_CATEGORY_RENDER, priority);
	SDL_SetLogPriority(LOG_CATEGORY_SYSTEM, priority);
	SDL_SetLogPriority(LOG_CATEGORY_TEST, priority);
	SDL_SetLogPriority(LOG_CATEGORY_VIDEO, priority);
	SDL_SetLogPriority(LOG_PRIORITY_CRITICAL, priority);
	return *this;
}
StandardFilePush::StandardFilePush(bool st) : iscreate_(st)
{
	if (iscreate_ == true)
	{
		std::lock_guard<std::mutex> lg(mtx_);
		std::ostringstream ost;
		ost << time(0) << "_Log.log";
		filename_ = ost.str();
		fs_.open(filename_);
		if (!(fs_.is_open()))
			throw logsis_init_error();
	}
}
StandardFilePush::StandardFilePush(const char* name) : iscreate_(true)
{
	filename_ = name;
	fs_.open(name);
	if (!(fs_.is_open()))
		throw logsis_init_error();
}
StandardLogStream& StandardLogStream::operator<<(std::string str)
{
	SDL_LogMessage(category_, (SDL_LogPriority)priority_, str.c_str());
	std::lock_guard<std::mutex> lg(mtx_);
	if (iscreate_ == true)
		fs_ << priority_str_[priority_] << category_str_[category_] << str << std::endl;
	return *this;
}
StandardLogStream& StandardLogStream::operator<<(const char* str)
{
	SDL_LogMessage(category_, (SDL_LogPriority)priority_, str);
	std::lock_guard<std::mutex> lg(mtx_);
	if (iscreate_ == true)
		fs_ << priority_str_[priority_] << category_str_[category_] << str << std::endl;
	return *this;
}
StandardLogStream& StandardLogStream::logIn(std::string str, LogPriority priority, LogCategory category)
{
	SDL_LogMessage(category, (SDL_LogPriority)priority, str.c_str());
	std::lock_guard<std::mutex> lg(mtx_);
	if (iscreate_ == true)
		fs_ << priority_str_[priority_] << category_str_[category_] << str << std::endl;
	return *this;
}
StandardLogStream& StandardLogStream::logIn(const char* str, LogPriority priority, LogCategory category)
{
	SDL_LogMessage(category, (SDL_LogPriority)priority, str);
	std::lock_guard<std::mutex> lg(mtx_);
	if (iscreate_ == true)
		fs_ << priority_str_[priority_] << category_str_[category_] << str << std::endl;
	return *this;
}

StandardLogStream lst;