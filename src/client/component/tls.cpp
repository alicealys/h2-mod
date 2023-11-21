#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "console.hpp"
#include "game_module.hpp"

#include <utils/io.hpp>
#include <utils/hook.hpp>

#pragma section("code_buf", read, execute)

namespace tls
{
	namespace
	{
		__declspec(allocate("code_buf")) char code_buffer[0x100]{};

		DWORD get_tls_index()
		{
			const auto game = game_module::get_game_module();
			const auto game_tls = reinterpret_cast<PIMAGE_TLS_DIRECTORY>(game.get_ptr() + game.get_optional_header()
				->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
			return *reinterpret_cast<DWORD*>(game_tls->AddressOfIndex);
		}

		void get_tls_stub(utils::hook::assembler& a)
		{
			std::uint8_t bytes[] = {0x65, 0x48, 0x8B, 0x04, 0x25, 0x58, 0x00, 0x00, 0x00}; // mov rax, gs:58h
			a.embed(bytes, sizeof(bytes));
			a.add(rax, get_tls_index() * 8);
			a.ret();
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Add tls_index * 8 offset to tls access where it's missing (it was optimized away in the bnet code)
			utils::hook::jump(code_buffer, utils::hook::assemble(get_tls_stub), true);

			static std::vector<std::size_t> address_list =
			{
				0x14004614C,
				0x140047A85,
				0x140048B4C,
				0x140049A04,
				0x140049DDE,
				0x140049FA9,
				0x14004A491,
				0x14004A879,
				0x14004AB34,
				0x14004AFE0,
				0x14004B305,
				0x14004B846,
				0x14004BC34,
				0x14004C024,
				0x14004C2CC,
				0x14004C94A,
				0x14004CBA1,
				0x1400619E9,
				0x1400A29CB,
				0x1400A2AF7,
				0x1400A2C23,
				0x1400A2D4F,
				0x1400A4A9F,
				0x1400A4EC5,
				0x1400A50CB,
				0x1400A53A8,
				0x1400A5618,
				0x1400A58A2,
				0x1400A5B08,
				0x1400A5D52,
				0x1400A5F44,
				0x1400A9B94,
				0x1400AA126,
				0x1400AA8D0,
				0x1400AACC6,
				0x1400AAF87,
				0x1400AB230,
				0x1400AB565,
				0x1400AB822,
				0x1400ABA43,
				0x1400ABCC9,
				0x1400ABED1,
				0x1400AC141,
				0x1400AC65B,
				0x1400AC833,
				0x1400ACA66,
				0x1400ACE27,
				0x1400AD048,
				0x1400AD51B,
				0x1400AD719,
				0x1400ADA2C,
				0x1400ADCD7,
				0x1400AE1B9,
				0x1400AE722,
				0x1400C795C,
				0x1400E3C93,
				0x1400E4165,
				0x1400E43F7,
				0x1400E46A7,
				0x1400F4951,
				0x1400F4BA6,
				0x1400F4DFF,
				0x140100938,
				0x140193761,
				0x1401DA392,
				0x1401FEA2A,
				0x1402039BB,
				0x14020C5A3,
			};

			for (const auto& address : address_list)
			{
				utils::hook::nop(address, 0x9);
				utils::hook::call(address, code_buffer);
			}
		}
	};
}

REGISTER_COMPONENT(tls::component)
