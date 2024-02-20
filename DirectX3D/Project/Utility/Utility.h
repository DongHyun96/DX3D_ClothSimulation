#pragma once

namespace Utility
{
	wstring GetExtension(wstring file);

	// 주의 - 한글은 깨질 것
	string ToString(wstring str);
	wstring ToWString(string str);

	bool StartsWith(string str, string start);
	bool StartsWith(wstring str, wstring start);

	string GetFileName(string file); // 파일과 확장자 이름만

	void CreateFolder(string file); // 특정 경로를 넣어 해당 경로에 폴더가 없으면 폴더를 만듦

	vector<string> SplitString(string origin, string token);

	float Min(const float& value1, const float& value2, const float& value3);
	float Max(const float& value1, const float& value2, const float& value3);
}