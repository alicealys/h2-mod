#include "std_include.hpp"
#include "context.hpp"
#include "error.hpp"

#include "event_handler.hpp"

namespace ui_scripting::lua
{
	event_handler::event_handler(sol::state& state)
		: state_(state)
	{
		auto event_listener_handle_type = state.new_usertype<event_listener_handle>("event_listener_handle");

		event_listener_handle_type["clear"] = [this](const event_listener_handle& handle)
		{
			this->remove(handle);
		};
	}

	void event_handler::dispatch(const event& event)
	{
		callbacks_.access([&](task_list& tasks)
		{
			this->merge_callbacks();

			for (auto i = tasks.begin(); i != tasks.end();)
			{
				if (i->event != event.name || i->element->id != event.element->id)
				{
					++i;
					continue;
				}

				if (!i->is_deleted)
				{
					handle_error(i->callback(sol::as_args(event.arguments)));
				}

				if (i->is_volatile || i->is_deleted)
				{
					i = tasks.erase(i);
				}
				else
				{
					++i;
				}
			}
		});
	}

	event_listener_handle event_handler::add_event_listener(event_listener&& listener)
	{
		const uint64_t id = ++this->current_listener_id_;
		listener.id = id;
		listener.is_deleted = false;

		new_callbacks_.access([&listener](task_list& tasks)
		{
			tasks.emplace_back(std::move(listener));
		});

		return {id};
	}

	void event_handler::clear()
	{
		callbacks_.access([&](task_list& tasks)
		{
			new_callbacks_.access([&](task_list& new_tasks)
			{
				new_tasks.clear();
				tasks.clear();
			});
		});
	}

	void event_handler::remove(const event_listener_handle& handle)
	{
		auto mask_as_deleted = [&](task_list& tasks)
		{
			for (auto& task : tasks)
			{
				if (task.id == handle.id)
				{
					task.is_deleted = true;
					break;
				}
			}
		};

		callbacks_.access(mask_as_deleted);
		new_callbacks_.access(mask_as_deleted);
	}

	void event_handler::merge_callbacks()
	{
		callbacks_.access([&](task_list& tasks)
		{
			new_callbacks_.access([&](task_list& new_tasks)
			{
				tasks.insert(tasks.end(), std::move_iterator<task_list::iterator>(new_tasks.begin()),
				             std::move_iterator<task_list::iterator>(new_tasks.end()));
				new_tasks = {};
			});
		});
	}
}
