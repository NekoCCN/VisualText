#pragma once
#ifndef VISUALTEXT_ASSETMANAGER_ENTRYPOINT_H
#define VISUALTEXT_ASSETMANAGER_ENTRYPOINT_H
#include "AssetPackStream/AssetPackStream.h"
#include "AssetPack/AssetPack.h"
#include <memory>
#include <map>
#include <mutex>
#include <cstdint>
#include <concurrent_queue.h>
#include <string>

namespace vtasset
{
	class AssetResolver
	{
	public:
		typedef std::pair<ProgramIndex, std::vector<std::pair<uint64_t, std::shared_ptr<char> > > > resolver_pair;
		AssetResolver(const std::string& path, const uint32_t buffer_size) : AP_(path), buffer_size_(buffer_size)
		{  }
		std::shared_ptr<std::condition_variable> getConditionVariable()
		{
			return cv_p_;
		}
		void stop()
		{
            will_exit_ = true;
		}
		void operator>>(resolver_pair& rp)
		{
			while (!resolver_queue_.try_pop(rp))
			{
				cv_p_->notify_one();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
		bool start();
		void clear()
		{
			resolver_queue_.clear();
		}
	private:
		Concurrency::concurrent_queue<resolver_pair> resolver_queue_;
		std::shared_ptr<std::condition_variable> cv_p_;
		std::unique_lock<std::mutex> lock_;
		AssetPack AP_;
		bool will_exit_ = false;
		uint32_t buffer_size_ = 5;
	};
}

#endif