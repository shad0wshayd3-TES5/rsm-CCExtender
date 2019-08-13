#include "Commands.h"

#include "Clear.h"
#include "Help.h"


namespace Commands
{
	void Install()
	{
		Clear::Register();
		Help::Register();
		_MESSAGE("Registered all console commands");
	}
}
