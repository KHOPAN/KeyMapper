#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>
#include <string>
#include "definition.h"

void ParseJSON(BYTE* data) {
	rapidjson::Document document;
	document.Parse(reinterpret_cast<char*>(data));
	free(data);

	if(document.HasParseError()) {
		rapidjson::ParseErrorCode code = document.GetParseError();
		LPCSTR errorMessage = rapidjson::GetParseError_En(code);
		MessageBoxA(NULL, errorMessage, "KeyMapper JSON Parsing Error", KEYMAPPER_ERROR);
		return;
	}

	if(!document.IsArray()) {
		MessageBoxW(NULL, L"JSON root must be JSON array!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return;
	}
}
