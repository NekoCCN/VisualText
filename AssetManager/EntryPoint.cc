#include "EntryPoint.h"
#include "MemoryBuffer/MemoryBuffer.h"

bool vtasset::AssetResolver::start()
{
	resolver_pair tmp_buf;
	MemoryBuffer MB;
	
	while (will_exit_ == false)
	{
		if (resolver_queue_.unsafe_size() < buffer_size_)
		{
			AP_ >> tmp_buf.first;
			tmp_buf.second.resize(tmp_buf.first.asset_list_index_size);
			for (int i = 0; i < tmp_buf.first.asset_list_index_size; ++i)
			{
				AP_.getMemoryBuffer(tmp_buf.first.asset_list_index[i], MB);
				tmp_buf.second[0] = { MB.getBufferByte(), MB.getBufferPoint() };
			}
			resolver_queue_.push(tmp_buf);
		}
		
		cv_p_->wait(lock_, [&]() -> bool
			{
				return resolver_queue_.unsafe_size() < buffer_size_;
			});
	}
}