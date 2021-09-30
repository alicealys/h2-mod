#include <std_include.hpp>
#include "stack_isolation.hpp"

namespace ui_scripting
{
	stack_isolation::stack_isolation()
	{
		const auto state = *game::hks::lua_state;

		this->top_ = state->m_apistack.top;
		this->base_ = state->m_apistack.base;
		this->alloc_top_ = state->m_apistack.alloc_top;
		this->bottom_ = state->m_apistack.bottom;

		state->m_apistack.top = this->stack_;
		state->m_apistack.base = state->m_apistack.top;
		state->m_apistack.alloc_top = &this->stack_[ARRAYSIZE(this->stack_) - 1];
		state->m_apistack.bottom = state->m_apistack.top;
	}

	stack_isolation::~stack_isolation()
	{
		const auto state = *game::hks::lua_state;

		state->m_apistack.top = this->top_;
		state->m_apistack.base = this->base_;
		state->m_apistack.alloc_top = this->alloc_top_;
		state->m_apistack.bottom = this->bottom_;
	}
}
