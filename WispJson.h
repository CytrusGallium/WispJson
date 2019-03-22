#ifndef WISPJSON_H_INCLUDED
#define WISPJSON_H_INCLUDED

#include <iostream>
#include <map>
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

struct KeyValuePair;
void PrettyPrintJson (string ParamJson);
bool JsonCheckSyntax (string ParamJson);
string GenerateTabs (int ParamAmount);
map<string, string> ParseJson (string ParamJson);
bool CheckJsonIfValueIsObject (string ParamJson);
KeyValuePair GetKeyValuePairFromJsonElement (string ParamJson);
void PrintMarker (string ParamMarker);
string CleanKeyFromQuotationMarks (string ParamKey);

struct KeyValuePair
{
    string Key;
    string Value;
};

#endif // WISPJSON_H_INCLUDED
