// SPDX-FileCopyrightText:  2026-2026 The DOSBox Staging Team
// SPDX-License-Identifier: GPL-2.0-or-later

#include "webserver.h"
#include "bridge.h"
#include "private/capture.h"

#include "capture/capture.h"
#include "http/http.h"
#include "json/json.h"

using json = nlohmann::json;

namespace Webserver {

void CaptureStatusCommand::Execute()
{
	capturing_audio = CAPTURE_IsCapturingAudio();
	capturing_video = CAPTURE_IsCapturingVideo();
	LOG_DEBUG("API: CaptureStatusCommand(audio=%d, video=%d)",
	          capturing_audio, capturing_video);
}

void CaptureStatusCommand::Get(const httplib::Request&, httplib::Response& res)
{
	CaptureStatusCommand cmd;
	cmd.WaitForCompletion();

	json j;
	j["audio"]["capturing"] = cmd.capturing_audio;
	j["video"]["capturing"] = cmd.capturing_video;
	send_json(res, j);
}

void StartAudioCaptureCommand::Execute()
{
	success         = CAPTURE_StartAudioCapture();
	capturing_audio = CAPTURE_IsCapturingAudio();
	LOG_DEBUG("API: StartAudioCaptureCommand(started=%d)", success);
}

void StartAudioCaptureCommand::Post(const httplib::Request&, httplib::Response& res)
{
	StartAudioCaptureCommand cmd;
	cmd.WaitForCompletion();

	json j;
	j["audio"]["capturing"] = cmd.capturing_audio;
	send_json(res, j);
}

void StopAudioCaptureCommand::Execute()
{
	success         = CAPTURE_StopAudioCapture();
	capturing_audio = CAPTURE_IsCapturingAudio();
	LOG_DEBUG("API: StopAudioCaptureCommand(stopped=%d)", success);
}

void StopAudioCaptureCommand::Post(const httplib::Request&, httplib::Response& res)
{
	StopAudioCaptureCommand cmd;
	cmd.WaitForCompletion();

	json j;
	j["audio"]["capturing"] = cmd.capturing_audio;
	send_json(res, j);
}

} // namespace Webserver
