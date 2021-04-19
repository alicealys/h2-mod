// https://github.com/momo5502/open-iw5

#include "stdinc.hpp"

namespace utils
{
	memory::allocator memory::mem_allocator_;

	memory::allocator::~allocator()
	{
		this->clear();
	}

	void memory::allocator::clear()
	{
		std::lock_guard _(this->mutex_);

		for (auto& data : this->pool_)
		{
			memory::free(data);
		}

		this->pool_.clear();
	}

	void memory::allocator::free(void* data)
	{
		std::lock_guard _(this->mutex_);

		const auto j = std::find(this->pool_.begin(), this->pool_.end(), data);
		if (j != this->pool_.end())
		{
			memory::free(data);
			this->pool_.erase(j);
		}
	}

	void memory::allocator::free(const void* data)
	{
		this->free(const_cast<void*>(data));
	}

	void* memory::allocator::allocate(const size_t length)
	{
		std::lock_guard _(this->mutex_);

		const auto data = memory::allocate(length);
		this->pool_.push_back(data);
		return data;
	}

	bool memory::allocator::empty() const
	{
		return this->pool_.empty();
	}

	/*char* memory::allocator::duplicate_string(const std::string& string)
	{
		std::lock_guard _(this->mutex_);

		const auto data = memory::duplicate_string(string);
		this->pool_.push_back(data);
		return data;
	}*/

	void* memory::allocate(const size_t length)
	{
		const auto data = calloc(length, 1);
		assert(data != nullptr);
		return data;
	}

	/*char* memory::duplicate_string(const std::string& string)
	{
		const auto new_string = allocate_array<char>(string.size() + 1);
		std::memcpy(new_string, string.data(), string.size());
		return new_string;
	}*/

	void memory::free(void* data)
	{
		if (data)
		{
			::free(data);
		}
	}

	void memory::free(const void* data)
	{
		free(const_cast<void*>(data));
	}

	bool memory::is_set(const void* mem, const char chr, const size_t length)
	{
		const auto mem_arr = static_cast<const char*>(mem);

		for (size_t i = 0; i < length; ++i)
		{
			if (mem_arr[i] != chr)
			{
				return false;
			}
		}

		return true;
	}

	memory::allocator* memory::get_allocator()
	{
		return &memory::mem_allocator_;
	}
}
