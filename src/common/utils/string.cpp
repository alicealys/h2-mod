#include "string.hpp"
#include <sstream>
#include <cstdarg>
#include <algorithm>

#include "nt.hpp"

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
		std::transform(text.begin(), text.end(), text.begin(), [](const unsigned char input)
		{
			return static_cast<char>(std::tolower(input));
		});

		return text;
	}

	std::string to_upper(std::string text)
	{
		std::transform(text.begin(), text.end(), text.begin(), [](const unsigned char input)
		{
			return static_cast<char>(std::toupper(input));
		});

		return text;
	}

	bool starts_with(const std::string& text, const std::string& substring)
	{
		return text.find(substring) == 0;
	}

	bool ends_with(const std::string& text, const std::string& substring)
	{
		if (substring.size() > text.size()) return false;
		return std::equal(substring.rbegin(), substring.rend(), text.rbegin());
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
		if (OpenClipboard(nullptr))
		{
			std::string data;

			auto* const clipboard_data = GetClipboardData(1u);
			if (clipboard_data)
			{
				auto* const cliptext = static_cast<char*>(GlobalLock(clipboard_data));
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

	void set_clipboard_data(const std::string& text)
	{
		const auto len = text.size() + 1;
		const auto mem = GlobalAlloc(GMEM_MOVEABLE, len);

		memcpy(GlobalLock(mem), text.data(), len);
		GlobalUnlock(mem);

		if (OpenClipboard(nullptr))
		{
			EmptyClipboard();
			SetClipboardData(CF_TEXT, mem);
			CloseClipboard();
		}
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

	std::string convert(const std::wstring& wstr)
	{
		std::string result;
		result.reserve(wstr.size());

		for (const auto& chr : wstr)
		{
			result.push_back(static_cast<char>(chr));
		}

		return result;
	}

	std::wstring convert(const std::string& str)
	{
		std::wstring result;
		result.reserve(str.size());

		for (const auto& chr : str)
		{
			result.push_back(static_cast<wchar_t>(chr));
		}

		return result;
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

	bool find_lower(const std::string& a, const std::string& b)
	{
		return to_lower(a).find(to_lower(b)) != std::string::npos;
	}

	std::string truncate(const std::string& text, const size_t length, const std::string& end)
	{
		return text.size() <= length
			? text
			: text.substr(0, length - end.size()) + end;
	}

	bool is_clean_number(const std::string& a)
	{
		auto it = a.begin();
		while (it != a.end() && std::isdigit(*it)) ++it;
		return !a.empty() && it == a.end();
	}

	bool strstr_lower(const char* a, const char* b)
	{
		const char* a_ = a;
		const char* b_ = b;

		while (*a_ != '\0' && *b_ != '\0')
		{
			if (std::tolower(*a_) == std::tolower(*b_))
			{
				b_++;
			}
			else
			{
				b_ = b;
			}

			a_++;
		}

		return *b_ == '\0';
	}
}
