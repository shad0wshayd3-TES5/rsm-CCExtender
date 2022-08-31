#include "Commands.h"

#include "BetaComment.h"
#include "Clear.h"
#include "Help.h"
#include "SetPerkPoints.h"

namespace Commands
{
	void Install()
	{
		BetaComment::Register();
		Clear::Register();
		Help::Register();
		SetPerkPoints::Register();
		logger::info(FMT_STRING("Registered all console commands"));
	}
}
