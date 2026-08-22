#pragma once

#include "CommandPipe/CommandPipe.h"
#include "SelectedRefColor/SelectedRefColor.h"

namespace Hooks
{
	void Install()
	{
		CommandPipe::InstallHooks();
		SelectedRefColor::InstallHooks();
		REX::INFO("Installed all hooks"sv);
	}
}
