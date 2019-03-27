#ifndef WISPJSON_H_INCLUDED
#define WISPJSON_H_INCLUDED

#define DEBUG FALSE

#include <iostream>
#include <map>
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

// Json Parsing
struct KeyValuePair;
bool JsonCheckSyntax (string ParamJson);
map<string, string> ParseJson (string ParamJson);

// Json String Utilities
bool CheckJsonIfValueIsObject (string ParamJson);
KeyValuePair GetKeyValuePairFromJsonElement (string ParamJson);
string CleanKeyFromQuotationMarks (string ParamKey);

// Visual Stuff
void PrettyPrintJson (string ParamJson);
string GenerateTabs (int ParamAmount);
void PrintMarker (string ParamMarker);
void DebugPrint(string ParamMessage);

struct KeyValuePair
{
    string Key;
    string Value;
};

void PrettyPrintJson (string ParamJson)
{
    int i = 0;
    int length = ParamJson.length();
    int currentLevel = 0;
    bool inString = false;
    bool forceNewLine = false;
    bool onKey = false;
    bool onValue = false;
    string currentKeyBuffer = "";
    string currentValueBuffer = "";
    bool lastWasGroupSymbol = false; // Group symbols are {} []
    char lastChar;

    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Print First Character
    if (ParamJson[i] == '[')
        {
            SetConsoleTextAttribute(hConsole, 12);
            currentLevel += 1;
            cout << "[";
            cout << endl << GenerateTabs(currentLevel);
            lastWasGroupSymbol = true;
        }
        else if (ParamJson[i] == ']')
        {
            SetConsoleTextAttribute(hConsole, 12);
            currentLevel -=1;
            cout << "]";

            if (ParamJson[i+1] != ',')
                cout << endl << GenerateTabs(currentLevel);

            lastWasGroupSymbol = true;
        }
        else if (ParamJson[i] == '{')
        {
            SetConsoleTextAttribute(hConsole, 11);
            currentLevel += 1;
            cout << "{";
            cout << endl << GenerateTabs(currentLevel);
            lastWasGroupSymbol = true;
        }
        else if (ParamJson[i] == '}')
        {
            SetConsoleTextAttribute(hConsole, 11);
            currentLevel -= 1;
            cout << "}";

            if (ParamJson[i+1] != ',')
                cout << endl << GenerateTabs(currentLevel);

            lastWasGroupSymbol = true;
        }
        else
        {
            return;
        }

    SetConsoleTextAttribute(hConsole, 7);

    // Print other characters
    for (i=1; i<length; i++)
    {
        int tmpLevel = currentLevel;
        bool forceNewLine = false;

        if (ParamJson[i] == '[')
        {
            SetConsoleTextAttribute(hConsole, 12);

            if (lastChar != ':')
                currentLevel += 1;

            cout << endl << GenerateTabs(currentLevel);
            cout << "[";

            lastWasGroupSymbol = true;

            if (lastChar == ':')
                currentLevel += 1;
            cout << endl << GenerateTabs(currentLevel);
        }
        else if (ParamJson[i] == ']')
        {
            SetConsoleTextAttribute(hConsole, 12);
            currentLevel -=1;
            cout << endl << GenerateTabs(currentLevel);
            cout << "]";

            if (ParamJson[i+1] != ',')
                cout << endl << GenerateTabs(currentLevel);

            lastWasGroupSymbol = true;
        }
        else if (ParamJson[i] == '{')
        {
            SetConsoleTextAttribute(hConsole, 11);

            if (lastChar != ':')
                currentLevel += 1;

            cout << endl << GenerateTabs(currentLevel);
            cout << "{";

            lastWasGroupSymbol = true;

            if (lastChar == ':')
                currentLevel += 1;
            cout << endl << GenerateTabs(currentLevel);
        }
        else if (ParamJson[i] == '}')
        {
            SetConsoleTextAttribute(hConsole, 11);
            currentLevel -= 1;
            cout << endl << GenerateTabs(currentLevel);
            cout << "}";

            if (ParamJson[i+1] != ',')
                cout << endl << GenerateTabs(currentLevel);

            lastWasGroupSymbol = true;
        }
        else if (ParamJson[i] == '"' && inString == false)
        {
            SetConsoleTextAttribute(hConsole, 10);
            inString = true;
            cout << "\"";
            lastWasGroupSymbol = false;
        }

        else if (ParamJson[i] == '"' && inString == true)
        {
            SetConsoleTextAttribute(hConsole, 10);
            cout << "\"";
            inString = false;
            lastWasGroupSymbol = false;
        }
        else if (ParamJson[i] == ',' && inString == false)
        {
            SetConsoleTextAttribute(hConsole, 13);
            cout << ",";
            cout << endl << GenerateTabs(currentLevel);
            lastWasGroupSymbol = false;
        }
        else
        {
            SetConsoleTextAttribute(hConsole, 7);
            cout << ParamJson[i];
            lastWasGroupSymbol = false;
        }

        lastChar = ParamJson[i];
    }

    SetConsoleTextAttribute(hConsole, 7);
    return;
}

bool JsonCheckSyntax (string ParamJson)
{
    // TODO : Find a way to check commas.
    // TODO : Find a way to check columns.

    if (ParamJson[0] == '{' || ParamJson[0] == '}')
    {
        // do nothing
    }
    else
    {
        return false;
    }

    int brackets = 0;
    int curlyBracers = 0;
    int quotationMarks = 0;

    int i = 0;
    int length = ParamJson.length();

    DebugPrint("Checking Json with the length of : " + length);

    for (i=0; i<length; i++)
    {
        if (ParamJson[i] == '[')
            brackets += 1;
        else if (ParamJson[i] == ']')
            brackets -=1;
        else if (ParamJson[i] == '{')
            curlyBracers += 1;
        else if (ParamJson[i] == '}')
            curlyBracers -= 1;
        else if (ParamJson[i] == '"')
            quotationMarks += 1;
    }

    if (brackets == 0 && curlyBracers == 0 && quotationMarks%2 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

    return false;
}

string GenerateTabs (int ParamAmount)
{
    string result = "";

    for (int i = 0; i < ParamAmount*4; i++)
    {
        result += " ";
    }

    return result;
}

map<string,string> ParseJson (string ParamJson)
{
    int i;
    int length = ParamJson.length();
    int brackets = 0;
    int curlyBracers = 0;
    string buffer;
    char startingCharacter;
    map<string, string> result;

    // I don't know why i need the starting character but i will store it.
    if (ParamJson[0] == '{')
    {
        startingCharacter = ParamJson[0];
        curlyBracers += 1;
    }
    else if (ParamJson[0] == '[')
    {
        startingCharacter = ParamJson[0];
        brackets += 1;
    }
    else
    {
        return result;
    }

    for (i=1; i<length; i++)
    {
        if (ParamJson[i] == '[')
            brackets += 1;
        else if (ParamJson[i] == ']')
            brackets -=1;
        else if (ParamJson[i] == '{')
            curlyBracers += 1;
        else if (ParamJson[i] == '}')
            curlyBracers -= 1;

        if (curlyBracers == 0 && brackets == 0)
            break;

        buffer += ParamJson[i];
    }

    DebugPrint(buffer);

    length = buffer.length();
    bool inString = false;
    vector<string> stringList;
    string elementBuffer;
    curlyBracers = 0;
    brackets = 0;

    // Generate a list of elements devided by commas
    for (i=0; i<length; i++)
    {
        if (buffer[i] == '"' && inString == false)
        {
            inString = true;
            elementBuffer += buffer[i];
        }
        else if (buffer[i] == '"' && inString == true)
        {
            inString = false;
            elementBuffer += buffer[i];
        }
        else if (buffer[i] == ',' && inString == false && brackets == 0 && curlyBracers == 0)
        {
            stringList.push_back(elementBuffer);
            elementBuffer = "";
        }
        else if (buffer[i] == ',' && inString == true)
        {
            elementBuffer += buffer[i];
        }
        else if (buffer[i] == '[')
        {
            brackets += 1;
            elementBuffer += buffer[i];
        }
        else if (buffer[i] == ']')
        {
            brackets -=1;
            elementBuffer += buffer[i];
        }
        else if (buffer[i] == '{')
        {
            curlyBracers += 1;
            elementBuffer += buffer[i];
        }
        else if (buffer[i] == '}')
        {
            curlyBracers -= 1;
            elementBuffer += buffer[i];
        }
        else
        {
            elementBuffer += buffer[i];
        }
    }

    stringList.push_back(elementBuffer);

    for(int i = 0; i < stringList.size(); i++)
    {
        KeyValuePair tmpPair = GetKeyValuePairFromJsonElement(stringList[i]);
        result.insert(pair<string,string>(tmpPair.Key,tmpPair.Value));
    }
}

bool CheckJsonIfValueIsObject (string ParamJson)
{
    int length = ParamJson.length();
    int i;
    bool inString = false;

    for (i=0; i<length; i++)
    {
        if (ParamJson[i] == '"' && inString == false)
        {
            inString = true;
        }
        else if (ParamJson[i] == '"' && inString == true)
        {
            inString = false;
        }
        else if (ParamJson[i] == ':' && inString == false)
        {
            if (ParamJson[i+1] == '{' || ParamJson[i+1] == '[')
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}

KeyValuePair GetKeyValuePairFromJsonElement (string ParamJson)
{
    int length = ParamJson.length();
    int i;
    bool inString = false;
    string keyBuffer;
    string valueBuffer;
    bool onKey = true;

    for (i=0; i<length; i++)
    {
        if (ParamJson[i] == '"' && inString == false)
        {
            inString = true;
            if (onKey)
                keyBuffer += ParamJson[i];
            else
                valueBuffer += ParamJson[i];
        }
        else if (ParamJson[i] == '"' && inString == true)
        {
            inString = false;
            if (onKey)
                keyBuffer += ParamJson[i];
            else
                valueBuffer += ParamJson[i];
        }
        else if (ParamJson[i] == ':' && inString == false && onKey == true)
        {
                onKey = false;
        }
        else
        {
            if (onKey)
                keyBuffer += ParamJson[i];
            else
                valueBuffer += ParamJson[i];
        }
    }

    KeyValuePair result;
    result.Key = CleanKeyFromQuotationMarks(keyBuffer);
    result.Value = CleanKeyFromQuotationMarks(valueBuffer);

    return result;
}

void PrintMarker (string ParamMarker)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 79);
    cout << ParamMarker << endl;
    SetConsoleTextAttribute(hConsole, 7);
}

string CleanKeyFromQuotationMarks (string ParamKey)
{
    string result = ParamKey;

    if (result[0] == '"' && result[result.length()-1] == '"')
    {
        result.erase(0, 1);
        result.erase(result.length()-1);
    }

    return result;
}

void DebugPrint(string ParamMessage)
{
    #if DEBUG == TRUE
    cout << ParamMessage << endl;
    #endif // If DEBUG
}

#endif // WISPJSON_H_INCLUDED
