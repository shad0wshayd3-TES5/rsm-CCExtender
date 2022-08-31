#pragma once

#include "CommandPipe/CommandPipe.h"
#include "SelectedRefColor/SelectedRefColor.h"

namespace Hooks
{
	void Install()
	{
		CommandPipe::InstallHooks();
		SelectedRefColor::InstallHooks();
		logger::info("Installed all hooks"sv);
	}
}
