/*
	Copyright (C) 2008 - 2022
	by Mark de Wever <koraq@xs4all.nl>
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#define GETTEXT_DOMAIN "wesnoth-lib"

#include "gui/widgets/helper.hpp"

#include "color.hpp"
#include "formula/callable.hpp"
#include "formula/string_utils.hpp"
#include "gui/core/log.hpp"
#include "sdl/point.hpp"
#include "gui/widgets/settings.hpp"
#include "sdl/rect.hpp"
#include "tstring.hpp"
#include "sdl/input.hpp" // get_mouse_location

#include <SDL2/SDL.h>

namespace gui2
{
font::pango_text::FONT_STYLE decode_font_style(const std::string& style)
{
	static const std::map<std::string, font::pango_text::FONT_STYLE> font_style_map {
		{"normal",    font::pango_text::STYLE_NORMAL},
		{"bold",      font::pango_text::STYLE_BOLD},
		{"italic",    font::pango_text::STYLE_ITALIC},
		{"underline", font::pango_text::STYLE_UNDERLINE},
	};

	if(style.empty()) {
		return font::pango_text::STYLE_NORMAL;
	}

	if(const auto i = font_style_map.find(style); i != font_style_map.end()) {
		return i->second;
	}

	ERR_GUI_G << "Unknown style '" << style << "', using 'normal' instead.";
	return font::pango_text::STYLE_NORMAL;
}

color_t decode_color(const std::string& color)
{
	return color_t::from_rgba_string(color);
}

PangoAlignment decode_text_alignment(const std::string& alignment)
{
	if(alignment == "center") {
		return PANGO_ALIGN_CENTER;
	} else if(alignment == "right") {
		return PANGO_ALIGN_RIGHT;
	}

	if(!alignment.empty() && alignment != "left") {
		ERR_GUI_E << "Invalid text alignment '" << alignment << "', falling back to 'left'.";
	}

	return PANGO_ALIGN_LEFT;
}

std::string encode_text_alignment(const PangoAlignment alignment)
{
	switch(alignment) {
		case PANGO_ALIGN_LEFT:
			return "left";
		case PANGO_ALIGN_RIGHT:
			return "right";
		case PANGO_ALIGN_CENTER:
			return "center";
	}

	assert(false);
	// FIXME: without this "styled_widget reaches end of non-void function" in release mode
	throw "Control should not reach this point.";
}

t_string missing_widget(const std::string& id)
{
	return t_string(VGETTEXT("Mandatory widget '$id' hasn't been defined.", {{"id", id}}));
}

void get_screen_size_variables(wfl::map_formula_callable& variable)
{
	variable.add("screen_width", wfl::variant(settings::screen_width));
	variable.add("screen_height", wfl::variant(settings::screen_height));
	variable.add("screen_pitch_microns", wfl::variant(settings::screen_pitch_microns));
	variable.add("gamemap_width", wfl::variant(settings::gamemap_width));
	variable.add("gamemap_height", wfl::variant(settings::gamemap_height));
	variable.add("gamemap_x_offset", wfl::variant(settings::gamemap_x_offset));
}

wfl::map_formula_callable get_screen_size_variables()
{
	wfl::map_formula_callable result;
	get_screen_size_variables(result);

	return result;
}

point get_mouse_position()
{
	return sdl::get_mouse_location();
}

std::string_view debug_truncate(std::string_view text)
{
	return text.substr(0, 15);
}

} // namespace gui2
