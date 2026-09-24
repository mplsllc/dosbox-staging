// SPDX-FileCopyrightText:  2026-2026 The DOSBox Staging Team
// SPDX-License-Identifier: GPL-2.0-or-later

#include "capture/capture.h"

#include <gtest/gtest.h>

namespace {

TEST(CaptureControl, InitialStateIsInactive)
{
	CAPTURE_StopAudioCapture();
	EXPECT_FALSE(CAPTURE_IsCapturingAudio());
}

TEST(CaptureControl, StartBeginsAudioCapture)
{
	CAPTURE_StopAudioCapture();
	EXPECT_FALSE(CAPTURE_IsCapturingAudio());

	EXPECT_TRUE(CAPTURE_StartAudioCapture());
	EXPECT_TRUE(CAPTURE_IsCapturingAudio());

	CAPTURE_StopAudioCapture();
	EXPECT_FALSE(CAPTURE_IsCapturingAudio());
}

TEST(CaptureControl, RepeatedStartReturnsFalseAndRemainsActive)
{
	CAPTURE_StopAudioCapture();
	EXPECT_TRUE(CAPTURE_StartAudioCapture());
	EXPECT_TRUE(CAPTURE_IsCapturingAudio());

	// Second start while already active should return false but remain active
	EXPECT_FALSE(CAPTURE_StartAudioCapture());
	EXPECT_TRUE(CAPTURE_IsCapturingAudio());

	CAPTURE_StopAudioCapture();
	EXPECT_FALSE(CAPTURE_IsCapturingAudio());
}

TEST(CaptureControl, StopFinalizesAudioCapture)
{
	CAPTURE_StopAudioCapture();
	EXPECT_TRUE(CAPTURE_StartAudioCapture());
	EXPECT_TRUE(CAPTURE_IsCapturingAudio());

	EXPECT_TRUE(CAPTURE_StopAudioCapture());
	EXPECT_FALSE(CAPTURE_IsCapturingAudio());
}

TEST(CaptureControl, RepeatedStopReturnsFalseAndRemainsInactive)
{
	CAPTURE_StopAudioCapture();
	EXPECT_FALSE(CAPTURE_IsCapturingAudio());

	// Stop when already inactive
	EXPECT_FALSE(CAPTURE_StopAudioCapture());
	EXPECT_FALSE(CAPTURE_IsCapturingAudio());
}

TEST(CaptureControl, DestroyResetsAudioCaptureState)
{
	CAPTURE_StopAudioCapture();
	EXPECT_TRUE(CAPTURE_StartAudioCapture());
	EXPECT_TRUE(CAPTURE_IsCapturingAudio());

	CAPTURE_Destroy();
	EXPECT_FALSE(CAPTURE_IsCapturingAudio());
}

} // namespace
