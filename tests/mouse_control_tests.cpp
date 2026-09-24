// SPDX-FileCopyrightText:  2026-2026 The DOSBox Staging Team
// SPDX-License-Identifier: GPL-2.0-or-later

#include "hardware/input/mouse.h"

#include <gtest/gtest.h>
#include <cmath>

namespace {

TEST(MouseControl, GetPositionDoesNotCrash)
{
	const auto pos = MOUSE_GetPosition();
	EXPECT_GE(pos.x_abs, 0.0f);
	EXPECT_GE(pos.y_abs, 0.0f);
	EXPECT_GE(pos.x_norm, 0.0f);
	EXPECT_LE(pos.x_norm, 1.0f);
	EXPECT_GE(pos.y_norm, 0.0f);
	EXPECT_LE(pos.y_norm, 1.0f);
}

TEST(MouseControl, SyntheticMovedNormalizedClamping)
{
	MOUSE_SyntheticMoved(-0.5f, 1.5f, true);
	const auto pos = MOUSE_GetPosition();
	EXPECT_FLOAT_EQ(pos.x_norm, 0.0f);
	EXPECT_FLOAT_EQ(pos.y_norm, 1.0f);
}

TEST(MouseControl, SyntheticMovedCoordinates)
{
	MOUSE_SyntheticMoved(0.5f, 0.5f, true);
	const auto pos = MOUSE_GetPosition();
	if (pos.resolution_x > 1 && pos.resolution_y > 1) {
		EXPECT_NEAR(pos.x_norm, 0.5f, 0.01f);
		EXPECT_NEAR(pos.y_norm, 0.5f, 0.01f);
	}
}

TEST(MouseControl, SyntheticRelativeMovedCoordinates)
{
	MOUSE_SyntheticMoved(0.2f, 0.2f, true);
	const auto pos1 = MOUSE_GetPosition();
	MOUSE_SyntheticRelativeMoved(10.0f, 15.0f);
	const auto pos2 = MOUSE_GetPosition();
	EXPECT_FLOAT_EQ(pos2.x_abs - pos1.x_abs, 10.0f);
	EXPECT_FLOAT_EQ(pos2.y_abs - pos1.y_abs, 15.0f);
}

TEST(MouseControl, SyntheticMovedAbsoluteCoordinates)
{
	MOUSE_SyntheticMoved(100.0f, 200.0f, false);
	const auto pos = MOUSE_GetPosition();
	EXPECT_FLOAT_EQ(pos.x_abs, 100.0f);
	EXPECT_FLOAT_EQ(pos.y_abs, 200.0f);
}

TEST(MouseControl, SyntheticButtonExecutesSafely)
{
	EXPECT_NO_THROW({
		MOUSE_SyntheticButton(MouseButtonId::Left, true);
		MOUSE_SyntheticButton(MouseButtonId::Left, false);
		MOUSE_SyntheticButton(MouseButtonId::Right, true);
		MOUSE_SyntheticButton(MouseButtonId::Right, false);
		MOUSE_SyntheticButton(MouseButtonId::Middle, true);
		MOUSE_SyntheticButton(MouseButtonId::Middle, false);
	});
}

} // namespace
