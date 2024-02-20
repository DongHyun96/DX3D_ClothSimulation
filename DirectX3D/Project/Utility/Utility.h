#pragma once

namespace Utility
{
	wstring GetExtension(wstring file);

	// ���� - �ѱ��� ���� ��
	string ToString(wstring str);
	wstring ToWString(string str);

	bool StartsWith(string str, string start);
	bool StartsWith(wstring str, wstring start);

	string GetFileName(string file); // ���ϰ� Ȯ���� �̸���

	void CreateFolder(string file); // Ư�� ��θ� �־� �ش� ��ο� ������ ������ ������ ����

	vector<string> SplitString(string origin, string token);

	float Min(const float& value1, const float& value2, const float& value3);
	float Max(const float& value1, const float& value2, const float& value3);
}