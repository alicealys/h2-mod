#include <std_include.hpp>
#include "functions.hpp"

#include "../../component/gsc.hpp"

#include "component/gsc/script_extension.hpp"
#include "component/gsc/script_loading.hpp"

#include <utils/string.hpp>

namespace scripting
{
	namespace
	{
		int find_function_index(const std::string& name, const bool prefer_global)
		{
			const auto target = utils::string::to_lower(name);
			auto const& first = gsc::gsc_ctx->func_map();
			auto const& second = gsc::gsc_ctx->meth_map();

			if (const auto itr = first.find(name); itr != first.end())
			{
				return static_cast<int>(itr->second);
			}

			if (const auto itr = second.find(name); itr != second.end())
			{
				return static_cast<int>(itr->second);
			}

			return -1;
		}

		script_function get_function_by_index(const unsigned index)
		{
			static const auto function_table = &gsc::func_table;
			static const auto method_table = 0x14B155890;

			if (index < 0x1000)
			{
				return reinterpret_cast<script_function*>(function_table)[index - 1];
			}

			return reinterpret_cast<script_function*>(method_table)[index - 0x8000];
		}

		unsigned int parse_token_id(const std::string& name)
		{
			if (name.starts_with("_ID"))
			{
				return static_cast<unsigned int>(std::strtol(name.substr(3).data(), nullptr, 10));
			}

			return 0;
		}
	}

	std::vector<std::string> find_token(unsigned int id)
	{
		std::vector<std::string> results;

		results.push_back(utils::string::va("_ID%i", id));
		results.push_back(utils::string::va("_id_%04X", id));
		results.push_back(gsc::gsc_ctx->token_name(id));

		return results;
	}

	std::string find_token_single(unsigned int id)
	{
		return gsc::gsc_ctx->token_name(id);
	}

	unsigned int find_token_id(const std::string& name)
	{
		const auto id = gsc::gsc_ctx->token_id(name);
		if (id)
		{
			return id;
		}

		const auto parsed_id = parse_token_id(name);
		if (parsed_id)
		{
			return parsed_id;
		}

		return game::SL_GetCanonicalString(name.data());
	}

	script_function find_function(const std::string& name, const bool prefer_global)
	{
		const auto index = find_function_index(name, prefer_global);
		if (index < 0)
		{
			return nullptr;
		}

		return get_function_by_index(index);
	}
}
