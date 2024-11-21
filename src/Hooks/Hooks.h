#pragma once

#include "CommandPipe/CommandPipe.h"
#include "SelectedRefColor/SelectedRefColor.h"

namespace Hooks
{
	void Install()
	{
		CommandPipe::InstallHooks();
		SelectedRefColor::InstallHooks();
		SKSE::log::info("Installed all hooks"sv);
	}
}
