#include "Hooks.h"

#include "CommandPipe.h"
#include "EditorIDCache.h"


namespace Hooks
{
	void Install()
	{
		CommandPipe::InstallHooks();
		EditorIDCache::InstallHooks();
		_MESSAGE("Installed all hooks");
	}
}
