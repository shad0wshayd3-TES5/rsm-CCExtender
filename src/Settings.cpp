#include "settings.h"

#include "Json2Settings.h"


bool Settings::LoadSettings(bool a_dumpParse)
{
	auto [log, success] = Json2Settings::load_settings(FILE_NAME, a_dumpParse);
	if (!log.empty()) {
		_ERROR("%s", log.c_str());
	}
	return success;
}


decltype(Settings::betaCommentFileName) Settings::betaCommentFileName("betaCommentFileName", "betacomments.txt");
