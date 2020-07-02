#pragma once

class EditorIDCache
{
public:
	static EditorIDCache* GetSingleton();
	static void InstallHooks();

	std::string GetEditorID(RE::TESForm* a_form) const;

private:
	using Lock = std::mutex;
	using Locker = std::scoped_lock<Lock>;
	using EditorID = std::string;

	EditorIDCache() = default;
	EditorIDCache(const EditorIDCache&) = delete;
	EditorIDCache(EditorIDCache&&) = delete;
	~EditorIDCache() = default;

	EditorIDCache& operator=(const EditorIDCache&) = delete;
	EditorIDCache& operator=(EditorIDCache&&) = delete;

	static void WritePatch(REL::ID a_hookID, std::uintptr_t a_funcAddr);
	static bool Hook_SetFormEditorID(RE::TESForm* a_this, const char* a_str);

	mutable Lock _lock;
	std::unordered_map<RE::FormID, EditorID> _idMap;
};
