#include "Framework.h"
#include "TerrainEditor.h"


TerrainEditor::TerrainEditor()
	:GameObject(L"06_TerrainEditor")
{
	material->SetDiffuseMap(L"Landscape/Fieldstone_DM.tga");
	material->SetSpecularMap(L"Landscape/Fieldstone_SM.tga");
	material->SetNormalMap(L"Landscape/Fieldstone_NM.tga");

	CreateMesh();

	computeShader = Shader::AddCS(L"07_ComputePicking");

	rayBuffer	  = new RayBuffer;
	brushBuffer	  = new BrushBuffer;
	
}

TerrainEditor::~TerrainEditor()
{
	delete brushBuffer;
	delete rayBuffer;
	delete structuredBuffer;
}

void TerrainEditor::Picking()
{
	Ray ray = CAMERA->ScreenPointToRay(mousePos);

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];

			index[0] = (x + 0) + (z + 0) * width;
			index[1] = (x + 1) + (z + 0) * width;
			index[2] = (x + 0) + (z + 1) * width;
			index[3] = (x + 1) + (z + 1) * width;

			Vector3 pos[4];
			
			for (UINT i = 0; i < 4; i++)
				pos[i] = vertices[index[i]].pos;

			float distance = 0.f;

			if (TriangleTests::Intersects(ray.origin, ray.direction, pos[0], pos[1], pos[2], distance))
			{
				pickedPos = ray.origin + ray.direction * distance;

				return;
			}

			if (TriangleTests::Intersects(ray.origin, ray.direction, pos[2], pos[1], pos[3], distance))
			{
				pickedPos = ray.origin + ray.direction * distance;

				return;
			}
		}
	}
}

void TerrainEditor::ComputePicking()
{
	Ray ray = CAMERA->ScreenPointToRay(mousePos);

	rayBuffer->data.origin			= ray.origin;
	rayBuffer->data.direction		= ray.direction;
	rayBuffer->data.polygonCount	= polygonCount;

	rayBuffer->SetCSBuffer(0);

	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	computeShader->SetShader();

	UINT x = ceilf(polygonCount / 64.f);

	DC->Dispatch(x, 1, 1); // dispatch - 보내다, 알리다 -> Compute shader의 draw call

	structuredBuffer->Copy(output.data(), sizeof(OutputDesc) * polygonCount);

	float minDistance	= FLT_MAX;
	int minIndex		= -1;

	UINT index = 0;

	for (OutputDesc data : output)
	{
		if (data.isPicked)
		{
			if (minDistance > data.distance)
			{
				minDistance = data.distance;
				minIndex = index;
			}
		}

		index++;
	}

	if (minIndex >= 0)
		pickedPos = ray.origin + ray.direction * minDistance;

	brushBuffer->data.pickedPos = pickedPos;
}

void TerrainEditor::Debug()
{
	if (ImGui::TreeNode("Terrain Editor Option"))
	{
		ImGui::Text("PickedPos: %.1f, %.1f, %.1f", pickedPos.x, pickedPos.y, pickedPos.z);

		const char* list[] = { "Height", "Alpha" };

		ImGui::Combo("EditorType", (int*)&type, list, 2);

		ImGui::DragFloat("BrushRange", &brushBuffer->data.range, 1, 1, 30);
		ImGui::DragFloat("AdjustValue", &adjustValue, 1, -50, 50);

		SaveHeightMap();

		//ImGui::SameLine();

		LoadHeightMap();

		material->SelectMap(&alphaMap, "AlphaMap", L"Solid/Red.png");

		material->SelectMap(&secondDiffuseMap, "SecondDiffuseMap", L"Solid/White.png");

		SaveAlphaMap();
		LoadAlphaMap();

		ImGui::TreePop();
	}

	if (DIALOG->Display("AlphaMap", 32, ImVec2(300, 100)) ||
		DIALOG->Display("SecondDiffuseMap", 32, ImVec2(300, 100)))
	{
		if (DIALOG->IsOk())
		{
			string path = DIALOG->GetFilePathName();

			path = path.substr(projectDir.length() + 1, path.length());

			if (DIALOG->GetOpenedKey() == "AlphaMap")
				alphaMap = Texture::Add(ToWString(path));
			else if (DIALOG->GetOpenedKey() == "SecondDiffuseMap")
				secondDiffuseMap = Texture::Add(ToWString(path));
		}

		DIALOG->Close();
	}
}

void TerrainEditor::Update()
{
	GameObject::Update();

	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (ImGui::GetIO().MouseDelta != ImVec2(0, 0))
			ComputePicking();

		switch (type)
		{
		case HEIGHT:
			if (KEY_PRESS(VK_LBUTTON))
				AdjustHeight();

			// Normal과 Tangent도 수정되기 때문에 다시금 계산 -> 높이 adjust할 때에는 normal과 tangent를 재계산 하지 않음
			if (KEY_UP(VK_LBUTTON))
				UpdateMesh();

			break;
		case ALPHA:
			if (KEY_PRESS(VK_LBUTTON))
				AdjustAlpha();
			break;
		default:
			break;
		}
	}
}

void TerrainEditor::Render(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	brushBuffer->SetPSBuffer(1);

	if (alphaMap)			alphaMap->PSSetShaderResources(10);
	if (secondDiffuseMap)	secondDiffuseMap->PSSetShaderResources(11);
	
	GameObject::Render(topology);
}

void TerrainEditor::CreateMesh()
{
	vector<Vector4> colors{};

	if (mesh) delete mesh;

	vertices.clear();
	indices.clear();

	if (heightMap)
	{
		colors = heightMap->ReadPixels();

		width = heightMap->GetSize().x;
		height = heightMap->GetSize().y;
	}

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;

			vertex.pos = Vector3(x, 0, height - z - 1);
			//vertex.pos = { x, 0, z };

			vertex.uv.x = (float)x / (width - 1);
			vertex.uv.y = (float)z / (height - 1);

			UINT index = x + width * z;

			if (!colors.empty())
				vertex.pos.y = colors[index].x * HEIGHT_SCALE;

			vertices.emplace_back(vertex);
		}
	}

	for (UINT i = 0; i < height - 1; i++)
	{
		for (UINT j = 0; j < width - 1; j++)
		{
			indices.emplace_back((i + 0) * width + (j + 0));
			indices.emplace_back((i + 0) * width + (j + 1));
			indices.emplace_back((i + 1) * width + (j + 0));

			indices.emplace_back((i + 1) * width + (j + 0));
			indices.emplace_back((i + 0) * width + (j + 1));
			indices.emplace_back((i + 1) * width + (j + 1));
		}
	}

	// Create Normal Vector
	for (UINT i = 0; i < indices.size() / 3; i++) // 폴리곤의 개수만큼 normal 생성
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v01 = vertices[index1].pos - vertices[index0].pos;
		Vector3 v02 = vertices[index2].pos - vertices[index0].pos;

		Vector3 normal = Vector3::Cross(v01, v02).GetNormalized();

		// 추후 평균을 냄
		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}

	CalculateTangent();

	mesh = new Mesh(vertices, indices);

	CreateCompute();
}

void TerrainEditor::AdjustHeight()
{
	switch (brushBuffer->data.type)
	{
	case 0:
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.pos.x, 0.f, vertex.pos.z);
			Vector3 p2 = Vector3(pickedPos.x, 0.f, pickedPos.z);

			float distance = Vector3(p1 - p2).Length();

			float value = adjustValue * max(0, cos(XM_PIDIV2 * distance / brushBuffer->data.range));

			if (distance <= brushBuffer->data.range)
			{
				vertex.pos.y += value * DELTA_TIME;
			}

			vertex.pos.y = Clamp(vertex.pos.y, 0.f, MAX_HEIGHT);

			// Normal이 바뀌기 때문에 초기화 한 뒤, 다시금 수정
			vertex.normal = Vector3(0, 1, 0);
			vertex.tangent = Vector3(1, 0, 0);
		}
		break;
	default:
		break;
	}

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::AdjustAlpha()
{
	for (VertexType& vertex : vertices)
	{
		Vector3 p1 = Vector3(vertex.pos.x, 0.f, vertex.pos.z);
		Vector3 p2 = Vector3(pickedPos.x, 0.f, pickedPos.z);

		float distance = Vector3(p1 - p2).Length();

		float value = adjustValue * max(0, cos(XM_PIDIV2 * distance / brushBuffer->data.range));

		if (distance <= brushBuffer->data.range)
		{
			vertex.alpha.x += value * DELTA_TIME;
		}

		vertex.alpha.x = Clamp(vertex.alpha.x, 0.f, 1.f);
	}
	mesh->UpdateVertex(vertices.data(), vertices.size());

}

void TerrainEditor::CalculateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		Vector2 uv0 = vertices[index0].uv;
		Vector2 uv1 = vertices[index1].uv;
		Vector2 uv2 = vertices[index2].uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float v0 = uv1.y - uv0.y;

		float u1 = uv2.x - uv0.x;
		float v1 = uv2.y - uv0.y;

		float D = 1.f / (u0 * v1 - v0 * u1);

		Vector3 tangent = D * (e0 * v1 - e1 * v0);

		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}

	// Tangent길이 보정
	for (VertexType& vertex : vertices)
	{
		Vector3 T = vertex.tangent;
		Vector3 N = vertex.normal;

		vertex.tangent = Vector3(T - N * Vector3::Dot(N, T)).GetNormalized();
	}
}

void TerrainEditor::SaveHeightMap()
{
	if (ImGui::Button("SaveHeightMap"))
		DIALOG->OpenDialog("SaveHeightMap", "SaveHeightMap", ".png", "_Texture/HeightMap/");

	if (DIALOG->Display("SaveHeightMap", 32, ImVec2(300, 100)))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			file = file.substr(projectDir.size() + 1, file.size());

			UINT size = width * height * 4;

			uint8_t* pixels = new uint8_t[size];

			for (UINT i = 0; i < size / 4; i++)
			{
				float vHeight = vertices[i].pos.y / MAX_HEIGHT * 255.f;

				pixels[i * 4 + 0] = vHeight;
				pixels[i * 4 + 1] = vHeight;
				pixels[i * 4 + 2] = vHeight;
				pixels[i * 4 + 3] = 255;
			}

			Image image{};

			image.width			= width;
			image.height		= height;
			image.pixels		= pixels;
			image.format		= DXGI_FORMAT_R8G8B8A8_UNORM;
			image.rowPitch		= width * 4;
			image.slicePitch	= size;

			SaveToWICFile
			(
				image,
				WIC_FLAGS_FORCE_RGB,
				GetWICCodec(WIC_CODEC_PNG),
				ToWString(file).c_str()
			);

			delete[] pixels;
		}

		DIALOG->Close();
	}
}

void TerrainEditor::LoadHeightMap()
{
	if (ImGui::Button("LoadHeightMap"))
		DIALOG->OpenDialog("LoadHeightMap", "LoadHeightMap", ".png", "_Texture/HeightMap/");

	if (DIALOG->Display("LoadHeightMap", 32, ImVec2(300, 100)))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			file = file.substr(projectDir.size() + 1, file.size());

			heightMap = Texture::Add(ToWString(file));

			CreateMesh();

		}

		DIALOG->Close();
	}
}

void TerrainEditor::SaveAlphaMap()
{
	if (ImGui::Button("SaveAlphaMap"))
		DIALOG->OpenDialog("SaveAlphaMap", "SaveAlphaMap", ".png", "_Texture/HeightMap/");

	if (DIALOG->Display("SaveAlphaMap", 32, ImVec2(300, 100)))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			file = file.substr(projectDir.size() + 1, file.size());

			UINT size = width * height * 4;

			uint8_t* pixels = new uint8_t[size];

			for (UINT i = 0; i < size / 4; i++)
			{
				pixels[i * 4 + 0] = vertices[i].alpha.x * 255.f;
				pixels[i * 4 + 1] = 0;
				pixels[i * 4 + 2] = 0;
				pixels[i * 4 + 3] = 255;
			}

			Image image{};

			image.width			= width;
			image.height		= height;
			image.pixels		= pixels;
			image.format		= DXGI_FORMAT_R8G8B8A8_UNORM;
			image.rowPitch		= width * 4;
			image.slicePitch	= size;

			SaveToWICFile
			(
				image,
				WIC_FLAGS_FORCE_RGB,
				GetWICCodec(WIC_CODEC_PNG),
				ToWString(file).c_str()
			);

			delete[] pixels;
		}

		DIALOG->Close();
	}
}

void TerrainEditor::LoadAlphaMap()
{
	if (ImGui::Button("LoadAlphaMap"))
		DIALOG->OpenDialog("LoadAlphaMap", "LoadAlphaMap", ".png", "_Texture/HeightMap/");

	if (DIALOG->Display("LoadAlphaMap", 32, ImVec2(300, 100)))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			file = file.substr(projectDir.size() + 1, file.size());

			Texture* alphaMap = Texture::Add(ToWString(file));

			vector<Vector4> pixels = alphaMap->ReadPixels();

			UINT size = min(pixels.size(), vertices.size());

			for (UINT i = 0; i < size; i++)
			{
				vertices[i].alpha.x = pixels[i].z;
				vertices[i].alpha.y = pixels[i].y;
				vertices[i].alpha.z = pixels[i].x;
				vertices[i].alpha.w = pixels[i].w;
			}

			mesh->UpdateVertex(vertices.data(), vertices.size());
		}

		DIALOG->Close();
	}
}

void TerrainEditor::UpdateMesh()
{
	for (UINT i = 0; i < indices.size() / 3; i++) // 폴리곤의 개수만큼 normal 생성
	{
		// Update Normal
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v01 = vertices[index1].pos - vertices[index0].pos;
		Vector3 v02 = vertices[index2].pos - vertices[index0].pos;

		Vector3 normal = Vector3::Cross(v01, v02).GetNormalized();

		// 추후 평균을 냄
		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;

		// Update Input
		input[i].v0 = vertices[index0].pos;
		input[i].v1 = vertices[index1].pos;
		input[i].v2 = vertices[index2].pos;

		// Update tangent
		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		Vector2 uv0 = vertices[index0].uv;
		Vector2 uv1 = vertices[index1].uv;
		Vector2 uv2 = vertices[index2].uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float v0 = uv1.y - uv0.y;

		float u1 = uv2.x - uv0.x;
		float v1 = uv2.y - uv0.y;

		float D = 1.f / (u0 * v1 - v0 * u1);

		Vector3 tangent = D * (e0 * v1 - e1 * v0);

		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}

	// Tangent길이 보정
	for (VertexType& vertex : vertices)
	{
		Vector3 T = vertex.tangent;
		Vector3 N = vertex.normal;

		vertex.tangent = Vector3(T - N * Vector3::Dot(N, T)).GetNormalized();
	}

	mesh->UpdateVertex(vertices.data(), vertices.size());

	structuredBuffer->UpdateInput(input.data());
}

void TerrainEditor::CreateCompute()
{
	polygonCount = indices.size() / 3;

	input.resize(polygonCount);
	output.resize(polygonCount);

	for (UINT i = 0; i < polygonCount; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		input[i].v0 = vertices[index0].pos;
		input[i].v1 = vertices[index1].pos;
		input[i].v2 = vertices[index2].pos;

	}

	if (structuredBuffer)
		delete structuredBuffer;

	structuredBuffer = new StructuredBuffer
	(
		input.data(),
		sizeof(InputDesc),
		input.size(),
		sizeof(OutputDesc),
		output.size()
	);
}
