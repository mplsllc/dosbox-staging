// SPDX-FileCopyrightText:  2026-2026 The DOSBox Staging Team
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DOSBOX_WEBSERVER_CAPTURE_H
#define DOSBOX_WEBSERVER_CAPTURE_H

#include "webserver/bridge.h"

#include "http/http.h"

namespace Webserver {

class CaptureStatusCommand : public Command {
public:
	bool capturing_audio = false;
	bool capturing_video = false;

	void Execute() override;
	static void Get(const httplib::Request& req, httplib::Response& res);
};

class StartAudioCaptureCommand : public Command {
public:
	bool success         = false;
	bool capturing_audio = false;

	void Execute() override;
	static void Post(const httplib::Request& req, httplib::Response& res);
};

class StopAudioCaptureCommand : public Command {
public:
	bool success         = false;
	bool capturing_audio = false;

	void Execute() override;
	static void Post(const httplib::Request& req, httplib::Response& res);
};

class TakeScreenshotCommand : public Command {
public:
	bool success = false;

	void Execute() override;
	static void Post(const httplib::Request& req, httplib::Response& res);
};

} // namespace Webserver

#endif // DOSBOX_WEBSERVER_CAPTURE_H
