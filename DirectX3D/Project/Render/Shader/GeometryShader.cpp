#include "Framework.h"
#include "GeometryShader.h"

GeometryShader::GeometryShader(wstring file)
{
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    // 여기에 GS가 shader의 진입점 함수 이름
    D3DCompileFromFile(file.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "GS", "gs_5_0", flags, 0, &blob, nullptr);

    DEVICE->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &geometryShader);
}

GeometryShader::~GeometryShader()
{
    geometryShader->Release();
}

void GeometryShader::SetShader()
{
    DC->GSSetShader(geometryShader, nullptr, 0);
}