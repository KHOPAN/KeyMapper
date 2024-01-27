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

	rapidjson::SizeType arraySize = document.Size();

	for(rapidjson::SizeType x = 0; x < arraySize; x++) {
		rapidjson::Value& keyboardEntry = document[x];

		if(!keyboardEntry.IsObject()) {
			MessageBoxW(NULL, L"JSON root entries must be JSON object!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
			return;
		}

		if(!keyboardEntry.HasMember("keyboard")) {
			MessageBoxW(NULL, L"JSON entry is missing 'keyboard' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
			return;
		}

		rapidjson::Value& keyboardField = keyboardEntry["keyboard"];

		if(!keyboardField.IsString()) {
			MessageBoxW(NULL, L"JSON entry 'keyboard' must be a string!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
			return;
		}

		if(!keyboardEntry.HasMember("mapping")) {
			MessageBoxW(NULL, L"JSON entry is missing 'mapping' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
			return;
		}

		rapidjson::Value& mappingField = keyboardEntry["mapping"];

		if(!mappingField.IsArray()) {
			MessageBoxW(NULL, L"JSON entry 'mapping' must be a JSON array!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
			return;
		}

		rapidjson::SizeType mappingSize = mappingField.Size();
		MappingData* mappingList = static_cast<MappingData*>(malloc(mappingSize * sizeof(MappingData)));

		for(rapidjson::SizeType y = 0; y < mappingSize; y++) {
			rapidjson::Value& mappingEntry = mappingField[y];

			if(!mappingEntry.IsObject()) {
				MessageBoxW(NULL, L"'mapping' entries must be JSON object!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				return;
			}

			if(!mappingEntry.HasMember("keyCode")) {
				MessageBoxW(NULL, L"'mapping' entry is missing 'keyCode' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				return;
			}

			rapidjson::Value& keyCodeField = mappingEntry["keyCode"];

			if(!keyCodeField.IsNumber()) {
				MessageBoxW(NULL, L"'mapping' entry 'keyCode' must be a number!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				return;
			}

			if(!mappingEntry.HasMember("dll")) {
				MessageBoxW(NULL, L"'mapping' entry is missing 'dll' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				return;
			}

			rapidjson::Value& dllField = mappingEntry["dll"];

			if(!dllField.IsString()) {
				MessageBoxW(NULL, L"'mapping' entry 'dll' must be a string!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				return;
			}

			if(!mappingEntry.HasMember("function")) {
				MessageBoxW(NULL, L"'mapping' entry is missing 'function' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				return;
			}

			rapidjson::Value& functionField = mappingEntry["function"];

			if(!functionField.IsString()) {
				MessageBoxW(NULL, L"'mapping' entry 'function' must be a string!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				return;
			}

			HMODULE library = LoadLibraryA(dllField.GetString());

			if(!library) {
				MessageBoxW(NULL, L"Unable to load DLL library!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				return;
			}

			VoidFunction function = reinterpret_cast<VoidFunction>(GetProcAddress(library, functionField.GetString()));
		
			if(!function) {
				MessageBoxW(NULL, L"Unable to load DLL function!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				return;
			}

			mappingList[y].keyCode = static_cast<USHORT>(keyCodeField.GetInt64());
			mappingList[y].function = function;
		}
	}
}
