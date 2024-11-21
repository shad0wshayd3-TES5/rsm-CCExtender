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
		SKSE::log::info("Registered all console commands"sv);
	}
}
