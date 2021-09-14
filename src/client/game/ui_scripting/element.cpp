#include <std_include.hpp>
#include "element.hpp"

#include <utils/string.hpp>

#define fps_font game::R_RegisterFont("fonts/fira_mono_regular.ttf", 25)

namespace ui_scripting
{
	namespace
	{
		uint64_t next_id;
		float screen_max[2];

		struct point
		{
			float x;
			float y;
			float f2;
			float f3;
		};

		struct rectangle
		{
			point p0;
			point p1;
			point p2;
			point p3;
		};

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

		void draw_image(float x, float y, float w, float h, float* transform, float* color, game::Material* material)
		{
			game::rectangle rect;

			rect.p0.x = x;
			rect.p0.y = y;
			rect.p0.f2 = 0.f;
			rect.p0.f3 = 1.f;

			rect.p1.x = x + w;
			rect.p1.y = y;
			rect.p1.f2 = 0.f;
			rect.p1.f3 = 1.f;

			rect.p2.x = x + w;
			rect.p2.y = y + h;
			rect.p2.f2 = 0.f;
			rect.p2.f3 = 1.f;

			rect.p3.x = x;
			rect.p3.y = y + h;
			rect.p3.f2 = 0.f;
			rect.p3.f3 = 1.f;

			game::R_DrawRectangle(&rect, transform[0], transform[1], transform[2], transform[3], color, material);
		}

		void check_resize()
		{
			screen_max[0] = game::ScrPlace_GetViewPlacement()->realViewportSize[0];
			screen_max[1] = game::ScrPlace_GetViewPlacement()->realViewportSize[1];
		}

		float relative(float value)
		{
			return (value / 1920.f) * screen_max[0];
		}

		int relative(int value)
		{
			return (int)(((float)value / 1920.f) * screen_max[0]);
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
			this->font = "default";
		}
		else
		{
			this->font = lowercase;
		}
	}

	void element::set_font(const std::string& _font)
	{
		const auto lowercase = utils::string::to_lower(_font);

		if (font_map.find(lowercase) == font_map.end())
		{
			this->font = "default";
		}
		else
		{
			this->font = lowercase;
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

	void element::set_glow_color(float r, float g, float b, float a)
	{
		this->glow_color[0] = r;
		this->glow_color[1] = g;
		this->glow_color[2] = b;
		this->glow_color[3] = a;
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

	void element::set_slice(float left_percent, float top_percent, float right_percent, float bottom_percent)
	{
		this->slice[0] = left_percent;
		this->slice[1] = top_percent;
		this->slice[2] = right_percent;
		this->slice[3] = bottom_percent;
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
		check_resize();

		if (this->background_color[3] > 0)
		{
			const auto background_material = game::Material_RegisterHandle(this->material.data());

			draw_image(
				relative(this->x) + relative(this->border_width[3]),
				relative(this->y) + relative(this->border_width[0]),
				relative(this->w),
				relative(this->h),
				(float*)this->slice,
				(float*)this->background_color,
				background_material
			);
		}

		if (this->border_color[3] > 0)
		{
			const auto _border_material = game::Material_RegisterHandle(this->border_material.data());

			draw_image(
				relative(this->x),
				relative(this->y),
				relative(this->w) + relative(this->border_width[1]) + relative(this->border_width[3]),
				relative(this->border_width[0]),
				(float*)this->slice,
				(float*)this->border_color,
				_border_material
			);

			draw_image(
				relative(this->x) + relative(this->border_width[3]) + relative(this->w),
				relative(this->y) + relative(this->border_width[0]),
				relative(this->border_width[1]),
				relative(this->h),
				(float*)this->slice,
				(float*)this->border_color,
				_border_material
			);

			draw_image(
				relative(this->x),
				relative(this->y) + relative(this->h) + relative(this->border_width[0]),
				relative(this->w) + relative(this->border_width[1]) + relative(this->border_width[3]),
				relative(this->border_width[2]),
				(float*)this->slice,
				(float*)this->border_color,
				_border_material
			);

			draw_image(
				relative(this->x),
				relative(this->y) + relative(this->border_width[0]),
				relative(this->border_width[3]),
				relative(this->h),
				(float*)this->slice,
				(float*)this->border_color,
				_border_material
			);
		}

		if (!this->text.empty())
		{
			const auto fontname = font_map[this->font];
			const auto _font = game::R_RegisterFont(fontname.data(), relative(this->fontsize));
			const auto text_width = game::R_TextWidth(this->text.data(), 0x7FFFFFFF, _font);

			auto _horzalign = get_align_value(this->horzalign, (float)text_width, relative(this->w));
			auto _vertalign = get_align_value(this->vertalign, (float)relative(this->fontsize), relative(this->h));

			game::CL_DrawTextPhysicalWithEffects(
				this->text.data(),
				0x7FFFFFFF,
				_font,
				relative(this->x) + relative(this->text_offset[0]) + _horzalign + relative(this->border_width[3]),
				relative(this->y) + relative(this->text_offset[1]) + _vertalign + relative(this->fontsize) + relative(this->border_width[0]),
				1.f, 1.f,
				(float*)this->color, 0,
				(float*)this->glow_color,
				0, 0, 0, 0, 0, 0, 0
			);
		}
	}
}
