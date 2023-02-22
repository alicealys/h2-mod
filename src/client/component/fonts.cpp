#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "fonts.hpp"
#include "console.hpp"
#include "filesystem.hpp"
#include "command.hpp"
#include "language.hpp"
#include "config.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>
#include <utils/memory.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>
#include <utils/image.hpp>
#include <utils/concurrency.hpp>

namespace fonts
{
	namespace
	{
		const char* hudelem_fonts[] =
		{
			"",
			"bigfixed",
			"smallfixed",
			"objective",
			"big",
			"small",
			"hudbig",
			"hudsmall",
			"buttomprompt",
			"subtitle",
			"timer",
			"nameplate",
			"bank",
			"bankshadow",
			"bankshadowmore",
		};

		struct font_data_t
		{
			std::unordered_map<std::string, game::TTF*> fonts;
			std::unordered_map<std::string, std::string> raw_fonts;
		};

		struct font_replacement
		{
			game::language_t language;
			std::string font;
			std::string replacement;
		};

		game::StringTable* get_font_replacements()
		{
			if (!game::DB_XAssetExists(game::ASSET_TYPE_STRINGTABLE, "font_replacements.csv"))
			{
				return nullptr;
			}

			return game::DB_FindXAssetHeader(game::ASSET_TYPE_STRINGTABLE, "font_replacements.csv", false).stringTable;
		}

		std::string get_font_replacement(const std::string& name)
		{
			const auto table = get_font_replacements();
			if (table == nullptr)
			{
				return name;
			}

			const auto current_language = language::current();
			const std::string language_str = game::languages[current_language].name;

			for (auto row = 0; row < table->rowCount; row++)
			{
				if (table->columnCount < 3)
				{
					continue;
				}
				
				const auto row_values = &table->values[(row * table->columnCount)];
				const auto lang = row_values[0].string;
				if (lang != language_str)
				{
					continue;
				}

				const auto font = row_values[1].string;
				const auto replacement = row_values[2].string;

				if (font == name)
				{
					return replacement;
				}
			}

			return name;
		}

		utils::concurrency::container<font_data_t> font_data;

		game::TTF* create_font(const std::string& name, const std::string& data)
		{
			const auto font = utils::memory::get_allocator()->allocate<game::TTF>();
			font->name = utils::memory::get_allocator()->duplicate_string(name);
			font->buffer = utils::memory::get_allocator()->duplicate_string(data);
			font->len = static_cast<int>(data.size());
			font->fontFace = 0;
			return font;
		}

		void free_font(game::TTF* font)
		{
			utils::memory::get_allocator()->free(font->buffer);
			utils::memory::get_allocator()->free(font->name);
			utils::memory::get_allocator()->free(font);
		}

		game::TTF* load_font(const std::string& name)
		{
			return font_data.access<game::TTF*>([&](font_data_t& data_) -> game::TTF*
			{
				if (const auto i = data_.fonts.find(name); i != data_.fonts.end())
				{
					return i->second;
				}

				std::string data{};
				if (const auto i = data_.raw_fonts.find(name); i != data_.raw_fonts.end())
				{
					data = i->second;
				}

				if (data.empty() && !filesystem::read_file(name, &data))
				{
					return nullptr;
				}

				const auto material = create_font(name, data);
				data_.fonts[name] = material;

				return material;
			});
		}

		game::TTF* try_load_font(const std::string& name)
		{
			try
			{
				return load_font(name);
			}
			catch (const std::exception& e)
			{
				console::error("Failed to load font %s: %s\n", name.data(), e.what());
			}

			return nullptr;
		}

		game::TTF* db_find_xasset_header_stub(game::XAssetType type, const char* name, int create_default)
		{
			const auto name_ = get_font_replacement(name);
			auto result = try_load_font(name_);

			if (result == nullptr)
			{
				result = game::DB_FindXAssetHeader(type, name_.data(), 0).ttf;
			}

			if (result == nullptr && (name != name_))
			{
				console::error("Couldn't find replacement font \"%s\" for \"%s\"\n", name_.data(), name);
			}

			if (result == nullptr)
			{
				result = game::DB_FindXAssetHeader(type, name, create_default).ttf;
			}

			return result;
		}

		game::Font_s* bank_font = nullptr;

		utils::hook::detour ui_get_font_handle_hook;
		utils::hook::detour ui_asset_cache_hook;

		game::Font_s* ui_get_font_handle_stub(void* a1, int font_index)
		{
			if (font_index < 12 || bank_font == nullptr)
			{
				return ui_get_font_handle_hook.invoke<game::Font_s*>(a1, font_index);
			}

			switch (font_index)
			{
			case 12:
			case 13:
			case 14:
				return bank_font;
			}

			return ui_get_font_handle_hook.invoke<game::Font_s*>(a1, font_index);
		}

		game::Font_s* get_bank_font()
		{
			if (language::is_asian())
			{
				return game::R_RegisterFont("fonts/defaultBold.otf", 32);
			}
			else
			{
				return game::R_RegisterFont("fonts/bank.ttf", 32);
			}
		}

		void* ui_asset_cache_stub()
		{
			bank_font = get_bank_font();
			return ui_asset_cache_hook.invoke<void*>();
		}

		int get_font_handle_index(int hudelem_font_index, int current)
		{
			switch (hudelem_font_index)
			{
			case 12:
			case 13:
			case 14:
				return hudelem_font_index;
			}

			return current;
		}

		void get_hud_elem_info_stub(utils::hook::assembler& a)
		{
			a.push(ebx);
			a.pushad64();
			a.mov(edx, ebx);
			a.mov(ecx, dword_ptr(rsi, 4));
			a.call_aligned(get_font_handle_index);
			a.mov(dword_ptr(rsp, 0x80), eax);
			a.popad64();
			a.pop(ebx);

			a.mov(edx, dword_ptr(rdi, 0x240));
			a.lea(r8, qword_ptr(rsp, 0x98));

			a.jmp(0x14037B39E);
		}

		int get_font_style(int font_index)
		{
			switch (font_index)
			{
			case 12:
				return 0; // none
			case 13:
				return 2; // shadowed
			case 14:
				return 4; // shadowed more
			}

			return 0;
		}

		void get_hud_elem_text_style_stub(utils::hook::assembler& a)
		{
			const auto original = a.newLabel();
			const auto loc_14037AF98 = a.newLabel();

			a.mov(eax, dword_ptr(rdi, 4));
			a.cmp(eax, 12);
			a.jl(original);
			
			a.push(edx);
			a.pushad64();

			a.mov(ecx, eax);
			a.call_aligned(get_font_style);
			a.mov(dword_ptr(rsp, 0x80), eax);

			a.popad64();
			a.pop(edx);

			a.jmp(0x14037AFA5);

			a.bind(original);

			a.cmp(eax, 9);
			a.jnz(loc_14037AF98);
			a.mov(edx, 0x400);
			a.jmp(0x14037AFA5);

			a.bind(loc_14037AF98);
			a.jmp(0x14037AF98);
		}
	}

	void add(const std::string& name, const std::string& data)
	{
		font_data.access([&](font_data_t& data_)
		{
			data_.raw_fonts[name] = data;
		});
	}

	void clear()
	{
		font_data.access([&](font_data_t& data_)
		{
			for (auto& font : data_.fonts)
			{
				free_font(font.second);
			}

			data_.fonts.clear();
			*reinterpret_cast<int*>(0x14EE3ACB8) = 0; // reset registered font count
		});
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::call(0x140747096, db_find_xasset_header_stub);

			// add custom fonts to hud elem fonts
			ui_asset_cache_hook.create(0x140606090, ui_asset_cache_stub);
			ui_get_font_handle_hook.create(0x1406058F0, ui_get_font_handle_stub);

			// change hudelem font array
			utils::hook::inject(0x1404C17A6, hudelem_fonts);
			utils::hook::set(0x1404C17B7, static_cast<int>(ARRAYSIZE(hudelem_fonts)));

			// handle custom fonts
			utils::hook::jump(0x14037B390, utils::hook::assemble(get_hud_elem_info_stub), true);
			// handle custom font styles
			utils::hook::jump(0x14037AF89, utils::hook::assemble(get_hud_elem_text_style_stub), true);

			command::add("dumpFont", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					return;
				}

				const auto name = params.get(1);
				const auto ttf = game::DB_FindXAssetHeader(game::XAssetType::ASSET_TYPE_TTF, name, false).ttf;
				if (ttf == nullptr)
				{
					console::error("Font does not exist\n");
					return;
				}

				const auto path = utils::string::va("dumps/%s", ttf->name);
				utils::io::write_file(path, std::string(ttf->buffer, ttf->len), false);
				console::info("Dumped to %s", path);
			});
		}
	};
}

REGISTER_COMPONENT(fonts::component)
