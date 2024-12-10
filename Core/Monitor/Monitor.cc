#include "Monitor.h"

SDL_Rect vtcore::MonitorLists::getDisplayBound(int32_t index) const
{
	SDL_Rect tmp;
	if (!(index <= monitor_num_ - 1 && index >= 0))  //index from 0 to monitor num - 1
	{
		lst.logIn("Invalid monitor index", logsys::LOG_PRIORITY_CRITICAL, logsys::LOG_CATEGORY_VIDEO);
		throw monitor_index_error();
	}
	SDL_GetDisplayBounds(monitor_list_[index], &tmp);
	return tmp;
}
void vtcore::MonitorLists::getDisplayBound(int32_t index, SDL_Rect* pt) const
{
	if (!(index <= monitor_num_ - 1 && index > 0))  //index from 0 to monitor num - 1
	{
		lst.logIn("Invalid monitor index", logsys::LOG_PRIORITY_CRITICAL, logsys::LOG_CATEGORY_VIDEO);
		throw monitor_index_error();
	}
	SDL_GetDisplayBounds(monitor_list_[index], pt);
}
std::vector<std::string> vtcore::MonitorLists::getMonitorNameList() const
{
	std::vector<std::string> tmp;
	for (int i = 0; i < monitor_num_; ++i)
		tmp.push_back(std::string(SDL_GetDisplayName(monitor_list_[i])));
	return tmp;
}
SDL_DisplayID vtcore::MonitorLists::operator[](int index) const
{
	if (!(index <= monitor_num_ - 1 && index > 0))  //index from 0 to monitor num - 1
	{
		lst.logIn("Invalid monitor index", logsys::LOG_PRIORITY_CRITICAL, logsys::LOG_CATEGORY_VIDEO);
		throw monitor_index_error();
	}
	return monitor_list_[index];
}
