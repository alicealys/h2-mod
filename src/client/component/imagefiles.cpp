#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "images.hpp"
#include "console.hpp"
#include "filesystem.hpp"
#include "fastfiles.hpp"
#include "scheduler.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/image.hpp>
#include <utils/io.hpp>
#include <utils/concurrency.hpp>

#define CUSTOM_IMAGE_FILE_INDEX 96

namespace imagefiles
{
	namespace
	{
		struct image_file_unk
		{
			char __pad0[88];
		};

		utils::memory::allocator image_file_allocator;
		std::unordered_map<std::string, image_file_unk*> image_file_unk_map;
		std::unordered_map<std::string, game::DB_IFileSysFile*> image_file_handles;

		std::string get_image_file_name()
		{
			return fastfiles::get_current_fastfile();
		}

		image_file_unk* get_image_file_unk(unsigned int index)
		{
			if (index != CUSTOM_IMAGE_FILE_INDEX)
			{
				return &reinterpret_cast<image_file_unk*>(0x144379510)[index];
			}

			const auto name = get_image_file_name();
			if (image_file_unk_map.find(name) == image_file_unk_map.end())
			{
				const auto unk = image_file_allocator.allocate<image_file_unk>();
				image_file_unk_map[name] = unk;
				return unk;
			}

			return image_file_unk_map[name];
		}

		game::DB_IFileSysFile* get_image_file_handle(unsigned int index)
		{
			if (index != CUSTOM_IMAGE_FILE_INDEX)
			{
				return reinterpret_cast<game::DB_IFileSysFile**>(0x14417B500)[index];
			}

			const auto name = get_image_file_name();
			return image_file_handles[name];
		}

		void db_create_gfx_image_stream_stub(utils::hook::assembler& a)
		{
			const auto check_image_file_handle = a.newLabel();
			const auto handle_is_open = a.newLabel();

			a.movzx(eax, cx);
			a.push(rax);
			a.push(rax);
			a.pushad64();
			a.mov(rcx, rax);
			a.call_aligned(get_image_file_unk);
			a.mov(qword_ptr(rsp, 0x80), rax);
			a.popad64();
			a.pop(rax);
			a.mov(r14, rax);
			a.pop(rax);

			a.push(rax);
			a.push(rax);
			a.pushad64();
			a.mov(rcx, rax);
			a.call_aligned(get_image_file_handle);
			a.mov(qword_ptr(rsp, 0x80), rax);
			a.popad64();
			a.pop(rax);
			a.mov(r12, rax);
			a.pop(rax);

			a.cmp(r12, r13);
			a.jnz(handle_is_open);
			a.jmp(0x14041CA91);

			a.bind(handle_is_open);
			a.jmp(0x14041CAE1);
		}

		void* pakfile_open_stub(void* /*handles*/, unsigned int count, int is_imagefile, 
			unsigned int index, int is_localized)
		{
			if (index != CUSTOM_IMAGE_FILE_INDEX)
			{
				return utils::hook::invoke<void*>(0x140622E80, 0x14417B500, count, is_imagefile, index, is_localized);
			}

			const auto name = get_image_file_name();
			const auto db_fs = game::DB_FSInitialize();
			const auto handle = db_fs->vftbl->OpenFile(db_fs, game::SF_PAKFILE, utils::string::va("%s.pak", name.data()));
			if (handle != nullptr)
			{
				image_file_handles[name] = handle;
			}
			return handle;
		}

		void com_sprintf_stub(char* buffer, const size_t len, const char* fmt, unsigned int index)
		{
			if (index != CUSTOM_IMAGE_FILE_INDEX)
			{
				return utils::hook::invoke<void>(0x140620480, buffer, len, fmt, index);
			}

			const auto name = get_image_file_name();
			utils::hook::invoke<void>(0x140620480, buffer, len, "%s.pak", name.data());
		}
	}

	void close_custom_handles()
	{
		const auto db_fs = game::DB_FSInitialize();
		for (const auto& handle : image_file_handles)
		{
			if (handle.second != nullptr)
			{
				db_fs->vftbl->Close(db_fs, handle.second);
			}
		}

		image_file_handles.clear();
		image_file_unk_map.clear();
		image_file_allocator.clear();
	}

	void close_handle(const std::string& fastfile)
	{
		if (!image_file_handles.contains(fastfile))
		{
			return;
		}

		const auto db_fs = game::DB_FSInitialize();
		const auto handle = image_file_handles[fastfile];
		if (handle != nullptr)
		{
			db_fs->vftbl->Close(db_fs, handle);
		}

		image_file_handles.erase(fastfile);
		image_file_allocator.free(image_file_unk_map[fastfile]);
		image_file_unk_map.erase(fastfile);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::jump(0x14041CA81, 
				utils::hook::assemble(db_create_gfx_image_stream_stub), true);
			utils::hook::call(0x14041CAC3, pakfile_open_stub);
			utils::hook::call(0x14041CAA5, com_sprintf_stub);
		}
	};
}

REGISTER_COMPONENT(imagefiles::component)
