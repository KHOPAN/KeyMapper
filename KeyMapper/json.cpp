#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>
#include <string>
#include "definition.h"

BOOL ProcessRootEntry(rapidjson::Value&, KeyMapStruct&);
BOOL ProcessKeyMapping(rapidjson::Value&, MappingData&);

BOOL ParseJSON(BYTE* data) {
	rapidjson::Document document;
	document.Parse(reinterpret_cast<char*>(data));
	free(data);

	if(document.HasParseError()) {
		LPCSTR errorMessage = rapidjson::GetParseError_En(document.GetParseError());
		MessageBoxA(NULL, errorMessage, "KeyMapper JSON Parsing Error", KEYMAPPER_ERROR);
		return 1;
	}

	if(!document.IsArray()) {
		MessageBoxW(NULL, L"Root JSON must be a JSON array!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	rapidjson::SizeType arraySize = document.Size();
	KeyMapStruct* structList = static_cast<KeyMapStruct*>(malloc(arraySize * sizeof(KeyMapStruct)));

	if(!structList) {
		DisplayError(ERROR_NOT_ENOUGH_MEMORY, L"malloc()");
		return 1;
	}

	for(rapidjson::SizeType i = 0; i < arraySize; i++) {
		if(ProcessRootEntry(document[i], structList[i])) {
			free(structList);
			return 1;
		}
	}

	KeyMappings mappings;
	mappings.keyMapStruct = structList;
	mappings.keyMapSize = static_cast<size_t>(arraySize);
	SetMappingData(mappings);
	return 0;
}

BOOL ProcessRootEntry(rapidjson::Value& keyboardEntry, KeyMapStruct& keyMapStruct) {
	if(!keyboardEntry.IsObject()) {
		MessageBoxW(NULL, L"Root JSON entry must be JSON objects!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	if(!keyboardEntry.HasMember("keyboard")) {
		MessageBoxW(NULL, L"Root JSON entry is missing a 'keyboard' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	rapidjson::Value& keyboardField = keyboardEntry["keyboard"];

	if(!keyboardField.IsString()) {
		MessageBoxW(NULL, L"Field 'keyboard' must be a string!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	if(!keyboardEntry.HasMember("mapping")) {
		MessageBoxW(NULL, L"Root JSON entry is missing a 'mapping' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	rapidjson::Value& mappingField = keyboardEntry["mapping"];

	if(!mappingField.IsArray()) {
		MessageBoxW(NULL, L"Field 'mapping' must be a JSON array!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	rapidjson::SizeType mappingSize = mappingField.Size();
	MappingData* mappingList = static_cast<MappingData*>(malloc(mappingSize * sizeof(MappingData)));

	if(!mappingList) {
		DisplayError(ERROR_NOT_ENOUGH_MEMORY, L"malloc()");
		return 1;
	}

	for(rapidjson::SizeType i = 0; i < mappingSize; i++) {
		if(ProcessKeyMapping(mappingField[i], mappingList[i])) {
			free(mappingList);
			return 1;
		}
	}

	CHAR* keyboardName = const_cast<CHAR*>(keyboardField.GetString());
	WCHAR* wideKeyboardName = WidenHeap(keyboardName);

	if(wideKeyboardName == NULL) {
		DisplayError(ERROR_NOT_ENOUGH_MEMORY, L"malloc()");
		return 1;
	}

	keyMapStruct.keyboardHuid = wideKeyboardName;
	keyMapStruct.mappings = mappingList;
	keyMapStruct.mappingSize = static_cast<size_t>(mappingSize);
	return 0;
}

BOOL ProcessKeyMapping(rapidjson::Value& mappingEntry, MappingData& mapping) {
	if(!mappingEntry.IsObject()) {
		MessageBoxW(NULL, L"Entry of field 'mapping' must be a JSON object!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	if(!mappingEntry.HasMember("keyCode")) {
		MessageBoxW(NULL, L"Entry of field 'mapping' is missing a 'keyCode' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	rapidjson::Value& keyCodeField = mappingEntry["keyCode"];

	if(!keyCodeField.IsNumber()) {
		MessageBoxW(NULL, L"Entry 'keyCode' of field 'mapping' must be a number!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	if(!mappingEntry.HasMember("dll")) {
		MessageBoxW(NULL, L"Entry of field 'mapping' is missing a 'dll' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	rapidjson::Value& dllField = mappingEntry["dll"];

	if(!dllField.IsString()) {
		MessageBoxW(NULL, L"Entry 'keyCode' of field 'mapping' must be a string!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	if(!mappingEntry.HasMember("function")) {
		MessageBoxW(NULL, L"Entry of field 'mapping' is missing a 'function' field!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	rapidjson::Value& functionField = mappingEntry["function"];

	if(!functionField.IsString()) {
		MessageBoxW(NULL, L"Entry 'function' of field 'mapping' must be a string!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	const char* libraryPath = dllField.GetString();
	HMODULE library = GetModuleHandleA(libraryPath);

	if(!library) {
		library = LoadLibraryA(libraryPath);
	}

	if(!library) {
		MessageBoxW(NULL, L"Unable to load the DLL library!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	VoidFunction function = reinterpret_cast<VoidFunction>(GetProcAddress(library, functionField.GetString()));

	if(!function) {
		MessageBoxW(NULL, L"Unable to load the DLL function!", L"KeyMapper JSON Error", KEYMAPPER_ERROR);
		return 1;
	}

	mapping.keyCode = static_cast<USHORT>(keyCodeField.GetInt64());
	mapping.function = function;
	return 0;
}
