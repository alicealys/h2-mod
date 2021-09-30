#include <std_include.hpp>
#include "types.hpp"
#include "execution.hpp"

namespace ui_scripting
{
	/***************************************************************
	 * Lightuserdata
	 **************************************************************/

	lightuserdata::lightuserdata(void* ptr_)
		: ptr(ptr_)
	{
	}

	/***************************************************************
	 * Userdata
	 **************************************************************/

	userdata::userdata(void* ptr_)
		: ptr(ptr_)
	{
	}

	void userdata::set(const value& key, const value& value) const
	{
		set_field(*this, key, value);
	}

	value userdata::get(const value& key) const
	{
		return get_field(*this, key);
	}

	arguments userdata::call(const std::string& name, const arguments& arguments) const
	{
		return call_method(this->ptr, name, arguments);
	}

	/***************************************************************
	 * Table
	 **************************************************************/

	table::table()
	{
		const auto state = *game::hks::lua_state;
		this->ptr = game::hks::Hashtable_Create(state, 0, 0);
	}

	table::table(game::hks::HashTable* ptr_)
		: ptr(ptr_)
	{
	}

	arguments table::get_keys() const
	{
		arguments keys{};
		auto current = this->ptr->m_hashPart;

		while (current->m_key.t)
		{
			keys.push_back(current->m_key);
			current++;
		}

		return keys;
	}

	unsigned int table::size() const
	{
		return static_cast<unsigned int>(this->get_keys().size());
	}

	void table::set(const value& key, const value& value) const
	{
		set_field(*this, key, value);
	}

	value table::get(const value& key) const
	{
		return get_field(*this, key);
	}

	/***************************************************************
	 * Function
	 **************************************************************/

	function::function(void* ptr_)
		: ptr(ptr_)
	{
	}

	arguments function::call(const arguments& arguments) const
	{
		call_script_function(*this, arguments);
		return {};
	}
}
