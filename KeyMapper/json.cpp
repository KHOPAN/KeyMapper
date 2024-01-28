#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>
#include <string>
#include "definition.h"

BOOL ParseJSON(BYTE* data) {
	rapidjson::Document document;
	document.Parse(reinterpret_cast<char*>(data));
	free(data);

	if(document.HasParseError()) {
		rapidjson::ParseErrorCode code = document.GetParseError();
		LPCSTR errorMessage = rapidjson::GetParseError_En(code);
		MessageBoxA(NULL, errorMessage, "KeyMapper JSON Parsing Error", KEYMAPPER_ERROR);
		return 1;
	}

	if(!document.IsArray()) {
		MessageBoxW(NULL, L"JSON root must be JSON array!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	rapidjson::SizeType arraySize = document.Size();
	KeyMapStruct* structList = static_cast<KeyMapStruct*>(malloc(arraySize * sizeof(KeyMapStruct)));

	if(!structList) {
		DisplayError(ERROR_NOT_ENOUGH_MEMORY, L"malloc()");
		return 1;
	}

	for(rapidjson::SizeType x = 0; x < arraySize; x++) {
		rapidjson::Value& keyboardEntry = document[x];

		if(!keyboardEntry.IsObject()) {
			MessageBoxW(NULL, L"JSON root entries must be JSON object!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
			free(structList);
			return 1;
		}

		if(!keyboardEntry.HasMember("keyboard")) {
			MessageBoxW(NULL, L"JSON entry is missing 'keyboard' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
			free(structList);
			return 1;
		}

		rapidjson::Value& keyboardField = keyboardEntry["keyboard"];

		if(!keyboardField.IsString()) {
			MessageBoxW(NULL, L"JSON entry 'keyboard' must be a string!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
			free(structList);
			return 1;
		}

		if(!keyboardEntry.HasMember("mapping")) {
			MessageBoxW(NULL, L"JSON entry is missing 'mapping' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
			free(structList);
			return 1;
		}

		rapidjson::Value& mappingField = keyboardEntry["mapping"];

		if(!mappingField.IsArray()) {
			MessageBoxW(NULL, L"JSON entry 'mapping' must be a JSON array!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
			free(structList);
			return 1;
		}

		rapidjson::SizeType mappingSize = mappingField.Size();
		MappingData* mappingList = static_cast<MappingData*>(malloc(mappingSize * sizeof(MappingData)));

		if(!mappingList) {
			DisplayError(ERROR_NOT_ENOUGH_MEMORY, L"malloc()");
			return 1;
		}

		for(rapidjson::SizeType y = 0; y < mappingSize; y++) {
			rapidjson::Value& mappingEntry = mappingField[y];

			if(!mappingEntry.IsObject()) {
				MessageBoxW(NULL, L"'mapping' entries must be JSON object!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				free(mappingList);
				free(structList);
				return 1;
			}

			if(!mappingEntry.HasMember("keyCode")) {
				MessageBoxW(NULL, L"'mapping' entry is missing 'keyCode' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				free(mappingList);
				free(structList);
				return 1;
			}

			rapidjson::Value& keyCodeField = mappingEntry["keyCode"];

			if(!keyCodeField.IsNumber()) {
				MessageBoxW(NULL, L"'mapping' entry 'keyCode' must be a number!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				free(mappingList);
				free(structList);
				return 1;
			}

			if(!mappingEntry.HasMember("dll")) {
				MessageBoxW(NULL, L"'mapping' entry is missing 'dll' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				free(mappingList);
				free(structList);
				return 1;
			}

			rapidjson::Value& dllField = mappingEntry["dll"];

			if(!dllField.IsString()) {
				MessageBoxW(NULL, L"'mapping' entry 'dll' must be a string!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				free(mappingList);
				free(structList);
				return 1;
			}

			if(!mappingEntry.HasMember("function")) {
				MessageBoxW(NULL, L"'mapping' entry is missing 'function' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				free(mappingList);
				free(structList);
				return 1;
			}

			rapidjson::Value& functionField = mappingEntry["function"];

			if(!functionField.IsString()) {
				MessageBoxW(NULL, L"'mapping' entry 'function' must be a string!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				free(mappingList);
				free(structList);
				return 1;
			}

			HMODULE library = LoadLibraryA(dllField.GetString());

			if(!library) {
				MessageBoxW(NULL, L"Unable to load DLL library!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				free(mappingList);
				free(structList);
				return 1;
			}

			VoidFunction function = reinterpret_cast<VoidFunction>(GetProcAddress(library, functionField.GetString()));
		
			if(!function) {
				MessageBoxW(NULL, L"Unable to load DLL function!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
				free(mappingList);
				free(structList);
				return 1;
			}

			mappingList[y].keyCode = static_cast<USHORT>(keyCodeField.GetInt64());
			mappingList[y].function = function;
		}

		CHAR* keyboardName = const_cast<CHAR*>(keyboardField.GetString());
		size_t length = strlen(keyboardName);
		CHAR* keyboardBuffer = static_cast<CHAR*>(malloc(sizeof(CHAR) * (length + 1)));

		if(keyboardBuffer == NULL) {
			DisplayError(ERROR_NOT_ENOUGH_MEMORY, L"malloc()");
			return 1;
		}

		for(int i = 0; i < length; i++) {
			keyboardBuffer[i] = keyboardName[i];
		}

		keyboardBuffer[length] = '\u0000';
		structList[x].keyboardHuid = keyboardBuffer;
		structList[x].mappings = mappingList;
		structList[x].mappingSize = static_cast<size_t>(mappingSize);
	}

	KeyMappings mappings;
	mappings.keyMapStruct = structList;
	mappings.keyMapSize = static_cast<size_t>(arraySize);
	SetMappingData(mappings);
	return 0;
}
