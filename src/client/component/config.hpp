#pragma once

namespace config
{
	nlohmann::json read_config();
	void write_config(const nlohmann::json& json);

	template <typename T>
	std::optional<T> get(const std::string& key)
	{
		const auto cfg = read_config();
		if (!cfg.is_object() || !cfg.contains(key))
		{
			return {};
		}

		return {cfg[key].get<T>()};
	}

	template <typename T>
	void set(const std::string& key, const T& value)
	{
		auto cfg = read_config();
		cfg[key] = value;
		write_config(cfg);
	}
}
