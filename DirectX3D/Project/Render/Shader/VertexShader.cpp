﻿#include "Framework.h"
#include "VertexShader.h"

VertexShader::VertexShader(wstring file)
{
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    D3DCompileFromFile(file.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", flags, 0, &blob, nullptr);

    DEVICE->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

    CreateInputLayout();        
}

VertexShader::~VertexShader()
{
    vertexShader->Release();
     inputLayout->Release();
            blob->Release();
}

void VertexShader::SetShader()
{
    DC->IASetInputLayout(inputLayout);
    DC->VSSetShader(vertexShader, nullptr, 0);
}

void VertexShader::CreateInputLayout()
{
    D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
    
    D3D11_SHADER_DESC shaderDesc;
    reflection->GetDesc(&shaderDesc);

    vector<D3D11_INPUT_ELEMENT_DESC> inputLayouts;

    for (UINT i = 0; i < shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        reflection->GetInputParameterDesc(i, &paramDesc);

        D3D11_INPUT_ELEMENT_DESC elementDesc;
        elementDesc.SemanticName         = paramDesc.SemanticName;
        elementDesc.SemanticIndex        = paramDesc.SemanticIndex;
        elementDesc.InputSlot            = 0;
        elementDesc.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
        elementDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        elementDesc.InstanceDataStepRate = 0;

        if (paramDesc.Mask == 1)
        {
                 if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)  elementDesc.Format = DXGI_FORMAT_R32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)  elementDesc.Format = DXGI_FORMAT_R32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
        }
        else if (paramDesc.Mask <= 3)
        {
                 if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)  elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
                 if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)  elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
        }
        else if (paramDesc.Mask <= 7)
        {
                 if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)  elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)  elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        }
        else if (paramDesc.Mask <= 15)
        {
                 if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)  elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)  elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }        

        string semantic = paramDesc.SemanticName;

        if (semantic == "POSITIONT")
            assert(!"Semantic is POSITIONT!");

        if (semantic == "POSITION")
            elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

        // semantic 앞에 INSTANCE 찾기
        int n = semantic.find_first_of("_");

        semantic = semantic.substr(0, n);

        if (semantic == "INSTANCE")
        {
            elementDesc.InputSlot               = 1;
            elementDesc.InputSlotClass          = D3D11_INPUT_PER_INSTANCE_DATA;
            elementDesc.InstanceDataStepRate    = 1;
        }

        inputLayouts.push_back(elementDesc);
    }   

    DEVICE->CreateInputLayout
    (
        inputLayouts.data(),
        inputLayouts.size(),
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        &inputLayout
    );
}
