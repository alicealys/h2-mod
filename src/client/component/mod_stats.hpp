#pragma once

#include <utils/concurrency.hpp>

namespace mod_stats
{
	using mod_stats_t = nlohmann::ordered_json;
	utils::concurrency::container<mod_stats_t>& get_stats();

	void initialize();
	void set_modified();
	void write();

	nlohmann::json get(const std::string& key, const nlohmann::json& default_value = {});
	nlohmann::json get_struct(const std::string& name, const std::string& field, const nlohmann::json& default_value = {});
	nlohmann::json get_all();

	void set(const std::string& key, const nlohmann::json& value);
	void set_struct(const std::string& name, const std::string& field, const nlohmann::json& value);
	void set_all(const nlohmann::json& value);
}
