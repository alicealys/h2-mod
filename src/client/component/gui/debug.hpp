namespace gui::debug
{
	size_t add_debug_line(const float* start, const float* end, const float* color);
	void set_debug_line_color(size_t line, const float* color);

	size_t add_debug_square(const float* origin, const float* color);
	void set_debug_square_color(size_t square, const float* color);

	void reset_debug_items();
}
