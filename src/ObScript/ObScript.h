#pragma once

#include "BetaComment/BetaComment.h"
#include "Clear/Clear.h"
#include "SetPerkPoints/SetPerkPoints.h"

namespace ObScript
{
	void Install()
	{
		BetaComment::Register();
		Clear::Register();
		SetPerkPoints::Register();
		logger::info("Registered all console commands"sv);
	}
}
