#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "fonts.hpp"
#include "console.hpp"
#include "filesystem.hpp"
#include "command.hpp"
#include "language.hpp"
#include "config.hpp"
#include "fastfiles.hpp"

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
		std::array<const char*, game::HE_FONT_COUNT> hudelem_fonts
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

		game::Font_s* bank_font = nullptr;

		utils::hook::detour ui_get_font_handle_hook;
		utils::hook::detour ui_get_font_handle2_hook;
		utils::hook::detour ui_asset_cache_hook;

		game::Font_s* get_font_handle_at_index(int font_index)
		{
			switch (font_index)
			{
			case game::HE_FONT_BANK:
			case game::HE_FONT_BANKSHADOW:
			case game::HE_FONT_BANKSHADOWMORE:
				return bank_font;
			}

			return nullptr;
		}

		game::Font_s* ui_get_font_handle_stub(void* a1, int font_index)
		{
			const auto res = get_font_handle_at_index(font_index);
			if (res)
			{
				return res;
			}

			return ui_get_font_handle_hook.invoke<game::Font_s*>(a1, font_index);
		}

		game::Font_s* ui_get_font_handle2_stub(void* a1, size_t a2)
		{
			const auto font_index = *reinterpret_cast<int*>(a2 + 208);
			const auto res = get_font_handle_at_index(font_index);
			if (res)
			{
				return res;
			}

			return ui_get_font_handle2_hook.invoke<game::Font_s*>(a1, a2);
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
			case game::HE_FONT_BANK:
			case game::HE_FONT_BANKSHADOW:
			case game::HE_FONT_BANKSHADOWMORE:
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
			case game::HE_FONT_BANK:
				return 0; // none
			case game::HE_FONT_BANKSHADOW:
				return 2; // shadowed
			case game::HE_FONT_BANKSHADOWMORE:
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

	void load_font_zones()
	{
		const auto disabled = config::get<bool>("disable_custom_fonts");
		const auto custom_fonts_disabled = disabled.has_value() && disabled.value() && !language::is_custom();
		const auto table = game::DB_FindXAssetHeader(game::ASSET_TYPE_STRINGTABLE, "font_zones.csv", 0).stringTable;
		if (table == nullptr)
		{
			return;
		}

		const auto lang = language::current();
		const std::string lang_name = game::languages[lang].name;
		for (auto row = 0; row < table->rowCount; row++)
		{
			if (table->columnCount < 3)
			{
				continue;
			}

			const auto row_values = &table->values[(row * table->columnCount)];
			const auto lang_value = row_values[0].string;
			const auto is_replacement = lang_value != "*"s;
			if (lang_value != lang_name && is_replacement)
			{
				continue;
			}

			if (custom_fonts_disabled && is_replacement)
			{
				continue;
			}

			for (auto col = 1; col < table->columnCount; col++)
			{
				const auto zone = row_values[col].string;
				if (zone != nullptr)
				{
					fastfiles::try_load_zone(zone, true);
				}
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// add custom fonts to hud elem fonts
			ui_asset_cache_hook.create(0x140606090, ui_asset_cache_stub);
			ui_get_font_handle_hook.create(0x1406058F0, ui_get_font_handle_stub);
			ui_get_font_handle2_hook.create(0x1405F9820, ui_get_font_handle2_stub);

			// change hudelem font array
			utils::hook::inject(0x1404C17A6, hudelem_fonts.data());
			utils::hook::set(0x1404C17B7, static_cast<int>(hudelem_fonts.size()));

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
				const auto ttf = game::DB_FindXAssetHeader(game::XAssetType::ASSET_TYPE_TTF, name, false).ttfDef;
				if (ttf == nullptr)
				{
					console::error("Font does not exist\n");
					return;
				}

				const auto path = utils::string::va("dumps/%s", ttf->name);
				utils::io::write_file(path, std::string(ttf->file, ttf->fileLen), false);
				console::info("Dumped to %s", path);
			});
		}
	};
}

REGISTER_COMPONENT(fonts::component)
