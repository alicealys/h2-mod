#include "http.hpp"
#include <algorithm>
#include <chrono>
#include <curl/curl.h>
#include <gsl/gsl>

#pragma comment(lib, "ws2_32.lib")

#ifdef max
#undef max
#endif

namespace utils::http
{
	namespace
	{
		struct progress_helper
		{
			const std::function<void(size_t, size_t, size_t)>* callback{};
			std::exception_ptr exception{};
			std::chrono::high_resolution_clock::time_point start{};
		};

		int progress_callback(void *clientp, const curl_off_t dltotal, const curl_off_t dlnow, const curl_off_t /*ultotal*/, const curl_off_t /*ulnow*/)
		{
			auto* helper = static_cast<progress_helper*>(clientp);

			try
			{
				const auto now = std::chrono::high_resolution_clock::now();
				const auto count = std::max(1, static_cast<int>(std::chrono::duration_cast<
					std::chrono::seconds>(now - helper->start).count()));
				const auto speed = dlnow / count;

				if (*helper->callback)
				{
					(*helper->callback)(dlnow, dltotal, speed);
				}
			}
			catch(...)
			{
				helper->exception = std::current_exception();
				return -1;
			}

			return 0;
		}
	
		size_t write_callback(void* contents, const size_t size, const size_t nmemb, void* userp)
		{
			auto* buffer = static_cast<std::string*>(userp);

			const auto total_size = size * nmemb;
			buffer->append(static_cast<char*>(contents), total_size);
			return total_size;
		}
	}

	std::optional<std::string> get_data(const std::string& url, const headers& headers, 
		const std::function<void(size_t, size_t, size_t)>& callback)
	{
		curl_slist* header_list = nullptr;
		auto* curl = curl_easy_init();
		if (!curl)
		{
			return {};
		}

		auto _ = gsl::finally([&]()
		{
			curl_slist_free_all(header_list);
			curl_easy_cleanup(curl);
		});
		
		for(const auto& header : headers)
		{
			auto data = header.first + ": " + header.second;
			header_list = curl_slist_append(header_list, data.data());
		}

		std::string buffer{};
		progress_helper helper{};
		helper.callback = &callback;
		helper.start = std::chrono::high_resolution_clock::now();
		
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &helper);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

		if (curl_easy_perform(curl) == CURLE_OK)
		{
			return {std::move(buffer)};
		}

		if (helper.exception)
		{
			std::rethrow_exception(helper.exception);
		}

		return {};
	}

	std::future<std::optional<std::string>> get_data_async(const std::string& url, const headers& headers)
	{
		return std::async(std::launch::async, [url, headers]()
		{
			return get_data(url, headers);
		});
	}
}
