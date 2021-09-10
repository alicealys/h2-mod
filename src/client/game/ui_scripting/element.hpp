#pragma once
#include "game/game.hpp"

namespace ui_scripting
{
	enum alignment
	{
		start,
		middle,
		end,
	};

	class element final
	{
	public:
		element();

		void set_horzalign(const std::string& value);
		void set_vertalign(const std::string& value);

		void set_text(const std::string& text);
		void set_font(const std::string& _font, const int _fontsize);
		void set_color(float r, float g, float b, float a);
		void set_text_offset(float x, float y);

		void set_background_color(float r, float g, float b, float a);
		void set_material(const std::string& material);

		void set_border_material(const std::string& material);
		void set_border_color(float r, float g, float b, float a);
		void set_border_width(float top);
		void set_border_width(float top, float right);
		void set_border_width(float top, float right, float bottom);
		void set_border_width(float top, float right, float bottom, float left);

		void set_rect(const float _x, const float _y, const float _w, const float _h);

		uint64_t id;

		void render() const;

		float x = 0.f;
		float y = 0.f;
		float w = 0.f;
		float h = 0.f;

		int fontsize = 20;

		float text_offset[2] = {0.f, 0.f};
		float color[4] = {1.f, 1.f, 1.f, 1.f};
		float background_color[4] = {0.f, 0.f, 0.f, 0.f};
		float border_color[4] = {0.f, 0.f, 0.f, 0.f};
		float border_width[4] = {0.f, 0.f, 0.f, 0.f};

		alignment horzalign = alignment::start;
		alignment vertalign = alignment::start;

		std::string font = "fonts/fira_mono_regular.ttf";
		std::string material = "white";
		std::string border_material = "white";
		std::string text{};
	};
}
