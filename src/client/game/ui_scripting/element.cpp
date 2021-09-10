#include <std_include.hpp>
#include "element.hpp"

#include <utils/string.hpp>

#define fps_font game::R_RegisterFont("fonts/fira_mono_regular.ttf", 25)

namespace ui_scripting
{
	namespace
	{
		uint64_t next_id;

		std::unordered_map<std::string, std::string> font_map =
		{
			{"bank", "fonts/bank.ttf"},
			{"fira_mono_bold", "fonts/fira_mono_bold.ttf"},
			{"fira_mono_regular", "fonts/fira_mono_regular.ttf"},
			{"defaultbold", "fonts/defaultbold.otf"},
			{"default", "fonts/default.otf"},
		};

		std::unordered_map<std::string, alignment> alignment_map =
		{
			{"left", alignment::start},
			{"top", alignment::start},
			{"center", alignment::middle},
			{"right", alignment::end},
			{"bottom", alignment::end},
		};

		float get_align_value(alignment align, float text_width, float w)
		{
			switch (align)
			{
			case (alignment::start):
				return 0.f;
			case (alignment::middle):
				return (w / 2.f) - (text_width / 2.f);
			case (alignment::end):
				return w - text_width;
			default:
				return 0.f;
			}
		}
	}

	element::element()
		: id(next_id++)
	{
	}

	void element::set_horzalign(const std::string& value)
	{
		const auto lower = utils::string::to_lower(value);
		if (alignment_map.find(lower) == alignment_map.end())
		{
			this->horzalign = alignment::start;
			return;
		}

		const auto align = alignment_map[lower];
		this->horzalign = align;
	}

	void element::set_vertalign(const std::string& value)
	{
		const auto lower = utils::string::to_lower(value);
		if (alignment_map.find(lower) == alignment_map.end())
		{
			this->vertalign = alignment::start;
			return;
		}

		const auto align = alignment_map[lower];
		this->vertalign = align;
	}

	void element::set_text(const std::string& _text)
	{
		this->text = _text;
	}

	void element::set_font(const std::string& _font, const int _fontsize)
	{
		this->fontsize = _fontsize;
		const auto lowercase = utils::string::to_lower(_font);

		if (font_map.find(lowercase) == font_map.end())
		{
			this->font = font_map["default"];
		}
		else
		{
			this->font = font_map[lowercase];
		}
	}

	void element::set_text_offset(float _x, float _y)
	{
		this->text_offset[0] = _x;
		this->text_offset[1] = _y;
	}

	void element::set_background_color(float r, float g, float b, float a)
	{
		this->background_color[0] = r;
		this->background_color[1] = g;
		this->background_color[2] = b;
		this->background_color[3] = a;
	}

	void element::set_color(float r, float g, float b, float a)
	{
		this->color[0] = r;
		this->color[1] = g;
		this->color[2] = b;
		this->color[3] = a;
	}

	void element::set_border_material(const std::string& _material)
	{
		this->border_material = _material;
	}

	void element::set_border_color(float r, float g, float b, float a)
	{
		this->border_color[0] = r;
		this->border_color[1] = g;
		this->border_color[2] = b;
		this->border_color[3] = a;
	}

	void element::set_border_width(float top)
	{
		this->border_width[0] = top;
		this->border_width[1] = top;
		this->border_width[2] = top;
		this->border_width[3] = top;
	}

	void element::set_border_width(float top, float right)
	{
		this->border_width[0] = top;
		this->border_width[1] = right;
		this->border_width[2] = top;
		this->border_width[3] = right;
	}

	void element::set_border_width(float top, float right, float bottom)
	{
		this->border_width[0] = top;
		this->border_width[1] = right;
		this->border_width[2] = bottom;
		this->border_width[3] = bottom;
	}

	void element::set_border_width(float top, float right, float bottom, float left)
	{
		this->border_width[0] = top;
		this->border_width[1] = right;
		this->border_width[2] = bottom;
		this->border_width[3] = left;
	}

	void element::set_material(const std::string& _material)
	{
		this->material = _material;
	}

	void element::set_rect(const float _x, const float _y, const float _w, const float _h)
	{
		this->x = _x;
		this->y = _y;
		this->w = _w;
		this->h = _h;
	}

	void element::render() const
	{
		const auto background_material = game::Material_RegisterHandle(this->material.data());
		game::R_AddCmdDrawStretchPic(
			this->x + this->border_width[3],
			this->y + this->border_width[0],
			this->w, this->h,
			0.0f, 0.0f, 0.0f, 0.0f, 
			(float*)this->background_color,
			background_material
		);

		const auto _border_material = game::Material_RegisterHandle(this->border_material.data());

		game::R_AddCmdDrawStretchPic(
			this->x,
			this->y,
			this->w + this->border_width[1] + this->border_width[3],
			this->border_width[0],
			0.f, 0.f, 0.f, 0.f, 
			(float*)this->border_color, 
			_border_material
		);

		game::R_AddCmdDrawStretchPic(
			this->x + this->border_width[3] + this->w,
			this->y + this->border_width[0],
			this->border_width[1],
			this->h,
			0.f, 0.f, 0.f, 0.f,
			(float*)this->border_color,
			_border_material
		);

		game::R_AddCmdDrawStretchPic(
			this->x,
			this->y + this->h + this->border_width[0],
			this->w + this->border_width[1] + this->border_width[3],
			this->border_width[2],
			0.f, 0.f, 0.f, 0.f,
			(float*)this->border_color,
			_border_material
		);

		game::R_AddCmdDrawStretchPic(
			this->x,
			this->y + this->border_width[0],
			this->border_width[3],
			this->h,
			0.f, 0.f, 0.f, 0.f,
			(float*)this->border_color,
			_border_material
		);

		if (!this->text.empty())
		{
			const auto _font = game::R_RegisterFont(this->font.data(), this->fontsize);
			const auto text_width = game::R_TextWidth(this->text.data(), 0x7FFFFFFF, _font);

			auto _horzalign = get_align_value(this->horzalign, (float)text_width, this->w);
			auto _vertalign = get_align_value(this->vertalign, (float)this->fontsize, this->h);

			game::R_AddCmdDrawText(this->text.data(), 0x7FFFFFFF, _font,
				this->x + this->text_offset[0] + _horzalign + this->border_width[3],
				this->y + this->text_offset[1] + _vertalign + this->fontsize + this->border_width[0],
				1.0f, 1.0f, 0.0f, 
				(float*)this->color,
				0
			);
		}
	}
}
