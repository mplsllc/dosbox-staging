// SPDX-FileCopyrightText:  2026-2026 The DOSBox Staging Team
// SPDX-License-Identifier: GPL-2.0-or-later

#include "webserver.h"
#include "bridge.h"
#include "private/mouse.h"

#include "hardware/input/mouse.h"
#include "http/http.h"
#include "json/json.h"
#include "misc/logging.h"
#include "utils/string_utils.h"

using json = nlohmann::json;

namespace Webserver {

void MouseStatusCommand::Execute()
{
	position = MOUSE_GetPosition();
	LOG_DEBUG("API: MouseStatusCommand(x=%.1f, y=%.1f)", position.x_abs, position.y_abs);
}

void MouseStatusCommand::Get(const httplib::Request&, httplib::Response& res)
{
	MouseStatusCommand cmd;
	cmd.WaitForCompletion();

	json j;
	j["x"]            = cmd.position.x_abs;
	j["y"]            = cmd.position.y_abs;
	j["normalized_x"] = cmd.position.x_norm;
	j["normalized_y"] = cmd.position.y_norm;
	j["resolution_x"] = cmd.position.resolution_x;
	j["resolution_y"] = cmd.position.resolution_y;
	send_json(res, j);
}

void MouseMoveCommand::Execute()
{
	if (rel_x.has_value() && rel_y.has_value() && !x.has_value() && !y.has_value()) {
		MOUSE_SyntheticRelativeMoved(*rel_x, *rel_y);
	} else if (x.has_value() && y.has_value()) {
		MOUSE_SyntheticMoved(*x, *y, normalized);
	}
	resulting_position = MOUSE_GetPosition();
	LOG_DEBUG("API: MouseMoveCommand(result x=%.1f, y=%.1f)",
	          resulting_position.x_abs, resulting_position.y_abs);
}

void MouseMoveCommand::Post(const httplib::Request& req, httplib::Response& res)
{
	MouseMoveCommand cmd;
	if (!req.body.empty()) {
		auto j = json::parse(req.body);
		if (j.contains("x")) {
			cmd.x = j["x"].get<float>();
		}
		if (j.contains("y")) {
			cmd.y = j["y"].get<float>();
		}
		if (j.contains("normalized")) {
			cmd.normalized = j["normalized"].get<bool>();
		}
		if (j.contains("rel_x")) {
			cmd.rel_x = j["rel_x"].get<float>();
		}
		if (j.contains("rel_y")) {
			cmd.rel_y = j["rel_y"].get<float>();
		}
	}

	if (!cmd.x.has_value() && !cmd.rel_x.has_value()) {
		throw std::invalid_argument("MouseMove requires 'x' and 'y', or 'rel_x' and 'rel_y'");
	}
	if (cmd.x.has_value() != cmd.y.has_value()) {
		throw std::invalid_argument("Both 'x' and 'y' must be specified together");
	}
	if (cmd.rel_x.has_value() != cmd.rel_y.has_value()) {
		throw std::invalid_argument("Both 'rel_x' and 'rel_y' must be specified together");
	}

	cmd.WaitForCompletion();

	json j;
	j["x"]            = cmd.resulting_position.x_abs;
	j["y"]            = cmd.resulting_position.y_abs;
	j["normalized_x"] = cmd.resulting_position.x_norm;
	j["normalized_y"] = cmd.resulting_position.y_norm;
	j["resolution_x"] = cmd.resulting_position.resolution_x;
	j["resolution_y"] = cmd.resulting_position.resolution_y;
	send_json(res, j);
}

static MouseButtonId parse_button_id(const json& btn_val)
{
	if (btn_val.is_number()) {
		int val = btn_val.get<int>();
		switch (val) {
		case 0: return MouseButtonId::Left;
		case 1: return MouseButtonId::Right;
		case 2: return MouseButtonId::Middle;
		default:
			throw std::invalid_argument("Invalid button index: " + std::to_string(val));
		}
	} else if (btn_val.is_string()) {
		std::string str = btn_val.get<std::string>();
		lowcase(str);
		if (str == "left" || str == "primary") {
			return MouseButtonId::Left;
		} else if (str == "right" || str == "secondary") {
			return MouseButtonId::Right;
		} else if (str == "middle") {
			return MouseButtonId::Middle;
		} else {
			throw std::invalid_argument("Invalid button name: " + str);
		}
	}
	throw std::invalid_argument("Button must be string or integer");
}

void MouseButtonCommand::Execute()
{
	MOUSE_SyntheticButton(button_id, pressed);
	LOG_DEBUG("API: MouseButtonCommand(button=%d, pressed=%d)",
	          static_cast<int>(button_id), pressed);
}

void MouseButtonCommand::Post(const httplib::Request& req, httplib::Response& res)
{
	auto j = json::parse(req.body);

	MouseButtonCommand cmd;
	if (j.contains("button")) {
		cmd.button_id = parse_button_id(j["button"]);
	} else {
		cmd.button_id = MouseButtonId::Left;
	}

	if (j.contains("pressed")) {
		cmd.pressed = j["pressed"].get<bool>();
	} else if (j.contains("down")) {
		cmd.pressed = j["down"].get<bool>();
	} else {
		throw std::invalid_argument("MouseButton requires 'pressed' boolean field");
	}

	cmd.WaitForCompletion();

	json out;
	std::string btn_name = (cmd.button_id == MouseButtonId::Left)    ? "left"
	                       : (cmd.button_id == MouseButtonId::Right) ? "right"
	                                                                 : "middle";
	out["button"]  = btn_name;
	out["pressed"] = cmd.pressed;
	send_json(res, out);
}

} // namespace Webserver
