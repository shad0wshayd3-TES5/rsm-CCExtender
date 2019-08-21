#include "Commands.h"

#include "BetaComment.h"
#include "Clear.h"
#include "Help.h"


namespace Commands
{
	void Install()
	{
		BetaComment::Register();
		Clear::Register();
		Help::Register();
		_MESSAGE("Registered all console commands");
	}
}
