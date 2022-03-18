#pragma once

#include "structs.hpp"
#include "launcher/launcher.hpp"

namespace game
{
	extern std::string mod_folder;

	namespace environment
	{
		launcher::mode get_mode();
		launcher::mode get_real_mode();

		bool is_sp();
		bool is_mp();
		bool is_dedi();

		void set_mode(launcher::mode mode);

		std::string get_string();
	}

	template <typename T>
	class symbol
	{
	public:
		symbol(const size_t address)
			: address_(reinterpret_cast<T*>(address))
		{
		}

		T* get() const
		{
			return reinterpret_cast<T*>(address_);
		}

		operator T* () const
		{
			return this->get();
		}

		T* operator->() const
		{
			return this->get();
		}

	private:
		T* address_;
	};
}

#include "symbols.hpp"
