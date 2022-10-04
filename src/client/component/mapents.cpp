#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/dvars.hpp"

#include "fastfiles.hpp"
#include "console.hpp"
#include "command.hpp"
#include "game/scripting/functions.hpp"

#include <xsk/gsc/types.hpp>
#include <xsk/resolver.hpp>

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/io.hpp>

namespace mapents
{
	namespace
	{
		std::string replace_mapents_keys(const std::string& data)
		{
			std::string buffer{};
			const auto lines = utils::string::split(data, '\n');

			for (const auto& line : lines)
			{
				const auto _0 = gsl::finally([&]
				{
					buffer.append("\n");
				});

				if (line.starts_with("{") || line.starts_with("}"))
				{
					buffer.append(line);
					continue;
				}

				const auto first_space = line.find_first_of(' ');
				if (first_space == std::string::npos)
				{
					continue;
				}

				const auto id = static_cast<unsigned int>(std::atoi(line.substr(0, first_space).data()));
				const auto token = xsk::gsc::h2::resolver::token_name(static_cast<std::uint16_t>(id));
				const auto key = "\"" + token + "\"";

				const auto new_line = key + line.substr(first_space);
				buffer.append(new_line);
			}

			return buffer;
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			command::add("dumpMapEnts", []()
			{
				if (!game::SV_Loaded())
				{
					console::info("Not in game\n");
					return;
				}

				fastfiles::enum_assets(game::ASSET_TYPE_MAP_ENTS, [](game::XAssetHeader header)
				{
					if (header.mapents == nullptr)
					{
						console::info("Failed to dump mapents\n");
						return;
					}

					const auto dest = utils::string::va("dumps/%s.ents", header.mapents->name);
					const auto str = std::string(header.mapents->entityString, header.mapents->numEntityChars);
					const auto data = replace_mapents_keys(str);
					utils::io::write_file(dest, data, false);
					console::info("Mapents dumped to %s\n", dest);
				}, true);
			});
		}
	};
}

REGISTER_COMPONENT(mapents::component)
