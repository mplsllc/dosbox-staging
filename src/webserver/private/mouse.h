// SPDX-FileCopyrightText:  2026-2026 The DOSBox Staging Team
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DOSBOX_WEBSERVER_MOUSE_H
#define DOSBOX_WEBSERVER_MOUSE_H

#include "webserver/bridge.h"
#include "hardware/input/mouse.h"
#include "http/http.h"

#include <optional>

namespace Webserver {

class MouseStatusCommand : public Command {
public:
	static void Get(const httplib::Request& req, httplib::Response& res);
	void Execute() override;

	MousePosition position = {};
};

class MouseMoveCommand : public Command {
public:
	static void Post(const httplib::Request& req, httplib::Response& res);
	void Execute() override;

	std::optional<float> x = std::nullopt;
	std::optional<float> y = std::nullopt;
	bool normalized = true;
	std::optional<float> rel_x = std::nullopt;
	std::optional<float> rel_y = std::nullopt;

	MousePosition resulting_position = {};
};

class MouseButtonCommand : public Command {
public:
	static void Post(const httplib::Request& req, httplib::Response& res);
	void Execute() override;

	MouseButtonId button_id = MouseButtonId::Left;
	bool pressed = false;
};

} // namespace Webserver

#endif // DOSBOX_WEBSERVER_MOUSE_H
