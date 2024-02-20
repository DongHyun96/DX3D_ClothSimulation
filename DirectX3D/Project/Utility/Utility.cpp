#include "Framework.h"
#include "Utility.h"

wstring Utility::GetExtension(wstring file)
{
    size_t index = file.find_last_of('.');

    return file.substr(index + 1, file.length());
}

string Utility::ToString(wstring str)
{
    string temp{};

    temp.assign(str.begin(), str.end());

    return temp;
}

wstring Utility::ToWString(string str)
{
    wstring temp{};

    temp.assign(str.begin(), str.end());

    return temp;
}

bool Utility::StartsWith(string str, string start)
{
    string result = str.substr(0, start.size());

    return result == start;
}

bool Utility::StartsWith(wstring str, wstring start)
{
    wstring result = str.substr(0, start.size());

    return result == start;
}

string Utility::GetFileName(string file)
{
    size_t cutAt = file.find_last_of('/');

    return file.substr(cutAt + 1);
}

void Utility::CreateFolder(string file)
{
    size_t cutAt = file.find_last_of('.');

    UINT size = 0;

    vector<string> folders = SplitString(file, "/");

    if (cutAt != file.npos)
        size = folders.size() - 1;
    else
        size = folders.size();


    string temp = "";

    for (UINT i = 0; i < size; i++)
    {
        temp += folders[i] + "/";

        if (!PathFileExistsA(temp.c_str()))
        {
            CreateDirectoryA(temp.c_str(), 0);
        }
    }
}

vector<string> Utility::SplitString(string origin, string token)
{
    vector<string> result;

    size_t cutAt = 0;

    while (cutAt != origin.npos)
    {
        cutAt = origin.find_first_of(token);

        if (cutAt > 0)
            result.push_back(origin.substr(0, cutAt));

        origin = origin.substr(cutAt + 1);
    }

    return result;
}

float Utility::Min(const float& value1, const float& value2, const float& value3)
{
    return min(min(value1, value2), value3);
}

float Utility::Max(const float& value1, const float& value2, const float& value3)
{
    return max(max(value1, value2), value3);
}
