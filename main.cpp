#include <iostream>
#include <map>
#include <string>
#include <windows.h>
#include <vector>
#include "WispJson/WispJson.h"

using namespace std;

int main()
{
    string testJson_1 = "{\"FirstName\":\"John\",\"LastName\":\"Doe\",\"Age\":43,\"Address\":{\"Street\":\"Downing Street 10\",\"City\":\"London\",\"Country\":\"Great Britain\"},\"Phone numbers\":[\"+44 1234567\",\"+44 2345678\"]}";
    string testJson_2 = "{\"Type\":\"EntityGrid\",\"Entities\":{\"1\":{\"PrimaryInfo\":\"The Internet\",\"SecondaryInfo\":\"\",\"ThirdiaryInfo\":\"\"},\"2\":{\"PrimaryInfo\":\"World War 1\",\"SecondaryInfo\":\"\",\"ThirdiaryInfo\":\"\"},\"3\":{\"PrimaryInfo\":\"World War 2\",\"SecondaryInfo\":\"\",\"ThirdiaryInfo\":\"\"}},\"stringSearchableProperties\":{\"NAME\":\"Designation\"}}";
    string testJson_3 = "{\"Type\"\"Entityid\",\"Entities\":{\"\":{\"PrimaryInfo\":\"Th\",\"SecondaryInfo\":\"\",\"ThirdiaryInfo\":\"\"},\"2\":{\"PrimaryInfo\":\"\",\"SecondaryInfo\":\"\",\"ThirdiaryInfo\":\"\"},\"3:{[\":{\"PrimaryInfo\":\"World War 2\",\"SecondaryInfo\":\"\",\"ThirdiaryInfo\":\"\"}},\"stringSearchableProperties\":{\"NAME\":\"Designation\"}}";

    // cout << JsonCheckSyntax(testJson_3);
    PrintMarker("PRETTY PRINT");
    PrettyPrintJson(testJson_2);
    cout << endl;
    map<string, string> json = ParseJson(testJson_2);
    cout << endl;

    PrintMarker("GET VALUE BY KEY");
    cout << json.at("Type");

    return 0;
}
