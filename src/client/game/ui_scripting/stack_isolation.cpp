#include <std_include.hpp>
#include "stack_isolation.hpp"

namespace ui_scripting
{
	stack_isolation::stack_isolation()
	{
		const auto state = *game::hks::lua_state;

		this->top_ = state->top;
		this->base_ = state->base;
		this->alloc_top_ = state->alloc_top;
		this->bottom_ = state->bottom;

		state->top = this->stack_;
		state->base = state->top;
		state->alloc_top = &this->stack_[ARRAYSIZE(this->stack_) - 1];
		state->bottom = state->top;
	}

	stack_isolation::~stack_isolation()
	{
		const auto state = *game::hks::lua_state;

		state->top = this->top_;
		state->base = this->base_;
		state->alloc_top = this->alloc_top_;
		state->bottom = this->bottom_;
	}
}
