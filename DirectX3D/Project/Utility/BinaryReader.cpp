#include "Framework.h"
#include "BinaryReader.h"


BinaryReader::BinaryReader(string path)
{
	file = CreateFileA
	(
		path.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ, 
		0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);
}

BinaryReader::BinaryReader(wstring path)
{
	file = CreateFile
	(
		path.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);
}

BinaryReader::~BinaryReader()
{
	CloseHandle(file);
}

int BinaryReader::ReadInt()
{
	int temp{};

	assert(ReadFile(file, &temp, sizeof(int), &size, nullptr));

	return temp;
}

UINT BinaryReader::ReadUint()
{
	UINT temp{};

	assert(ReadFile(file, &temp, sizeof(UINT), &size, nullptr));

	return temp;
}

float BinaryReader::ReadFloat()
{
	float temp{};

	assert(ReadFile(file, &temp, sizeof(float), &size, nullptr));

	return temp;
}

string BinaryReader::ReadString()
{
	UINT length = ReadUint();

	char* data = new char[length + 1];

	assert(ReadFile(file, data, sizeof(char) * length, &size, nullptr));

	data[length] = '\0';

	string temp = data;
	delete[] data;

	return temp;
}

wstring BinaryReader::ReadWstring()
{
	UINT length = ReadUint();

	WCHAR* data = new WCHAR[length + 1];

	assert(ReadFile(file, data, sizeof(WCHAR) * length, &size, nullptr));

	data[length] = '\0';

	wstring temp = data;
	delete[] data;

	return temp;
}

Vector3 BinaryReader::ReadVector3()
{
	Vector3 temp{};

	temp.x = ReadFloat();
	temp.y = ReadFloat();
	temp.z = ReadFloat();

	return temp;
}

Vector4 BinaryReader::ReadVector4()
{
	Vector4 temp{};

	temp.x = ReadFloat();
	temp.y = ReadFloat();
	temp.z = ReadFloat();
	temp.w = ReadFloat();

	return temp;
}

Matrix BinaryReader::ReadMatrix()
{
	Matrix temp{};

	assert(ReadFile(file, &temp, sizeof(Matrix), &size, nullptr));

	return temp;
}

void BinaryReader::ReadData(void** data, UINT dataSize)
{
	assert(ReadFile(file, *data, dataSize, &size, nullptr));
}
