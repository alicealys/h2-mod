// iw6x-client

#include <stdinc.hpp>

#include "memory.hpp"
#include "string.hpp"

namespace utils::string
{
	const char* va(const char* fmt, ...)
	{
		static thread_local va_provider<8, 256> provider;

		va_list ap;
		va_start(ap, fmt);

		const char* result = provider.get(fmt, ap);

		va_end(ap);
		return result;
	}

	std::vector<std::string> split(const std::string& s, const char delim)
	{
		std::stringstream ss(s);
		std::string item;
		std::vector<std::string> elems;

		while (std::getline(ss, item, delim))
		{
			elems.push_back(item); // elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
		}

		return elems;
	}

	std::string to_lower(std::string text)
	{
		std::transform(text.begin(), text.end(), text.begin(), [](const char input)
		{
			return CHAR(tolower(input));
		});

		return text;
	}

	std::string to_upper(std::string text)
	{
		std::transform(text.begin(), text.end(), text.begin(), [](const char input)
		{
			return CHAR(toupper(input));
		});

		return text;
	}

	bool starts_with(const std::string& text, const std::string& substring)
	{
		return text.find(substring) == 0;
	}

	std::string dump_hex(const std::string& data, const std::string& separator)
	{
		std::string result;

		for (unsigned int i = 0; i < data.size(); ++i)
		{
			if (i > 0)
			{
				result.append(separator);
			}

			result.append(va("%02X", data[i] & 0xFF));
		}

		return result;
	}

	std::string get_clipboard_data()
	{
		if (OpenClipboard(0))
		{
			std::string data;

			const auto clipboard_data = GetClipboardData(1u);
			if (clipboard_data)
			{
				const auto cliptext = static_cast<char*>(GlobalLock(clipboard_data));
				if (cliptext)
				{
					data.append(cliptext);
					GlobalUnlock(clipboard_data);
				}
			}
			CloseClipboard();

			return data;
		}
		return {};
	}

	void strip(const char* in, char* out, int max)
	{
		if (!in || !out) return;

		max--;
		auto current = 0;
		while (*in != 0 && current < max)
		{
			const auto color_index = (*(in + 1) - 48) >= 0xC ? 7 : (*(in + 1) - 48);

			if (*in == '^' && (color_index != 7 || *(in + 1) == '7'))
			{
				++in;
			}
			else
			{
				*out = *in;
				++out;
				++current;
			}

			++in;
		}
		*out = '\0';
	}

	std::string replace(std::string str, const std::string& from, const std::string& to)
	{
		if (from.empty())
		{
			return str;
		}

		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}

		return str;
	}
}
