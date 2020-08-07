#include "Hooks.h"

#include "CommandPipe.h"
#include "EditorIDCache.h"
#include "SelectedRefColor.h"

namespace Hooks
{
	void Install()
	{
		CommandPipe::InstallHooks();
		EditorIDCache::InstallHooks();
		SelectedRefColor::InstallHooks();
		logger::info(FMT_STRING("Installed all hooks"));
	}
}
