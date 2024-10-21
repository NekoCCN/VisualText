#pragma once
#ifndef VISUALTEXT_CORE_MONITOR_MONITOR_H_
#define VISUALTEXT_CORE_MONITOR_MONITOR_H_
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <vector>
#include <string>
#include <Core/Exception/Exception.h>
#include <Core/LogSystem/LogSystem.h>

class MonitorLists
{
private:
	int32_t monitor_num_;
	SDL_DisplayID* monitor_list_;
public:
	MonitorLists()
	{
		monitor_num_ = SDL_GetNumVideoDrivers();
		if (monitor_num_ < 1)
		{
			lst.logIn("Could found any monitor.Do you have a monitor?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_VIDEO);
		}
		monitor_list_ = SDL_GetDisplays(nullptr);
	}
	int32_t getMonitorNum()
	{
		return monitor_num_;
	}
	SDL_Rect getDisplayBound(int32_t index);  //index from 0 to monitor num - 1
	void getDisplayBound(int32_t index, SDL_Rect* pt);
	void refresh()
	{
		monitor_num_ = SDL_GetNumVideoDrivers();
		if (monitor_num_ < 1)
		{
			lst.logIn("Could found any monitor.Do you have a monitor?", logsys::LOG_PRIORITY_ERROR, logsys::LOG_CATEGORY_VIDEO);
		}
		monitor_list_ = SDL_GetDisplays(nullptr);
	}
	std::vector<SDL_DisplayID> getMonitorLists()
	{
		return std::vector<SDL_DisplayID>(monitor_list_, monitor_list_ + monitor_num_ - 1);
	}
	const char* getMonitorName(int32_t index)
	{
		if (!(index <= monitor_num_ - 1 && index > 0))  //index from 0 to monitor num - 1
		{
			lst.logIn("Invalid monitor index", logsys::LOG_PRIORITY_CRITICAL, logsys::LOG_CATEGORY_VIDEO);
			throw monitor_index_error();
		}
		return SDL_GetDisplayName(monitor_list_[index]);
	}
	std::vector<std::string> getMonitorNameList();
	SDL_DisplayID operator[](int index);
	std::vector<SDL_Rect> getMonitorBounds()
	{
		std::vector<SDL_Rect> tmp;
		for (int i = 0; i < monitor_num_; i++) 
		{
			tmp.push_back(SDL_Rect());
			SDL_GetDisplayBounds(i, &(tmp.back()));
		}
		return tmp;
	}
};
#endif