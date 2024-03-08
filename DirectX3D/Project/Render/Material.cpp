#include "Framework.h"
#include "Material.h"

UINT Material::index{};

Material::Material()
{
	name = "Material_" + to_string(index++);

	buffer = new MaterialBuffer;
}

Material::Material(wstring file)
{
	SetShader(file);

	name = "Material_" + to_string(index++);

	buffer = new MaterialBuffer;
}

Material::~Material()
{
	// Shader에서 delete 처리
	delete buffer;
}

void Material::Set()
{
	vertexShader->SetShader();
	pixelShader->SetShader();

	buffer->SetPSBuffer(4);

	// SRV
	if (diffuseMap)
		diffuseMap->PSSetShaderResources(0);

	if (specularMap)
		specularMap->PSSetShaderResources(1);

	if (normalMap)
		normalMap->PSSetShaderResources(2);

}

void Material::SetShader(wstring file)
{
	vertexShader = Shader::AddVS(file);
	pixelShader = Shader::AddPS(file);

}

void Material::SetDiffuseMap(wstring file)
{
	diffuseMap = Texture::Add(file);

	buffer->data.hasDiffuseMap = true;
}

void Material::SetDiffuseMap(Texture* texture)
{
	diffuseMap = texture;

	buffer->data.hasDiffuseMap = true;
}

void Material::SetSpecularMap(wstring file)
{
	specularMap = Texture::Add(file);

	buffer->data.hasSpecularMap = true;
}

void Material::SetNormalMap(wstring file)
{
	normalMap = Texture::Add(file);

	buffer->data.hasNormalMap = true;
}

void Material::Debug(string baseDir)
{
	string diffuseKey	= name + "DiffuseMap";
	string specularKey	= name + "SpecularMap";
	string normalKey	= name + "NormalMap";


	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::ColorEdit4("Diffuse",  (float*)&buffer->data.diffuse,  ImGuiColorEditFlags_PickerHueBar);
		ImGui::ColorEdit4("Specular", (float*)&buffer->data.specular, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::ColorEdit4("Ambient",  (float*)&buffer->data.ambient);
		ImGui::ColorEdit4("Emissive", (float*)&buffer->data.emissive);

		ImGui::SliderFloat("Shininess", &buffer->data.shininess, 1, 50);


		SelectMap(&diffuseMap,  diffuseKey,	 L"Solid/White.png");
		SelectMap(&specularMap, specularKey, L"Solid/Black.png");
		SelectMap(&normalMap,   normalKey,	 L"Solid/White.png");
		
		SaveDialog(baseDir);
		LoadDialog(baseDir);

		ImGui::TreePop();
	}

	if (DIALOG->Display(diffuseKey, 32, ImVec2(300, 100))
		|| DIALOG->Display(specularKey, 32, ImVec2(300, 100)) 
		|| DIALOG->Display(normalKey, 32, ImVec2(300, 100)))
	{
		if (DIALOG->IsOk())
		{
			string path = DIALOG->GetFilePathName();

			path = path.substr(projectDir.length() + 1, path.length());

			if (DIALOG->GetOpenedKey() == diffuseKey)
				SetDiffuseMap(ToWString(path));
			else if (DIALOG->GetOpenedKey() == specularKey)
				SetSpecularMap(ToWString(path));
			else if (DIALOG->GetOpenedKey() == normalKey)
				SetNormalMap(ToWString(path));
		}

		DIALOG->Close();
	}
}

void Material::SelectMap(Texture** texture, string mapType, wstring clearFile)
{
	ImTextureID textureID = Texture::Add(clearFile, ToWString(mapType) + clearFile)->GetSRV();

	if (*texture)
		textureID = (*texture)->GetSRV();

	ImGui::Text(mapType.c_str());

	if (ImGui::ImageButton(textureID, ImVec2(50, 50)))
		DIALOG->OpenDialog(mapType, mapType, ".png,.jpg,.tga,.dds,.bmp,.TGA,.PNG,.JPG,.DDS", "_Texture/");

	ImGui::SameLine();

	textureID = Texture::Add(L"Misc/X_Image.png", ToWString(mapType) + L"X_Image")->GetSRV();

	if (ImGui::ImageButton(textureID, ImVec2(20, 20)))
		*texture = Texture::Add(clearFile, ToWString(mapType) + clearFile);
}

void Material::SaveTransform(wstring file)
{
	BinaryWriter data(file);

	data.WriteData(name);

	if (vertexShader) data.WriteData(vertexShader->GetPath());
	else data.WriteData("");

	if (pixelShader) data.WriteData(pixelShader->GetPath());
	else data.WriteData("");

	if (diffuseMap) data.WriteData(diffuseMap->GetPath());
	else data.WriteData("");

	if (specularMap) data.WriteData(specularMap->GetPath());
	else data.WriteData("");

	if (normalMap) data.WriteData(normalMap->GetPath());
	else data.WriteData("");

	data.WriteData(buffer->data.diffuse);
	data.WriteData(buffer->data.specular);
	data.WriteData(buffer->data.ambient);
	data.WriteData(buffer->data.emissive);
	data.WriteData(buffer->data.shininess);
}

void Material::LoadTransform(wstring file)
{
	BinaryReader data(file);

	name = data.ReadString();

	wstring str{};

	str = data.ReadWstring();
	if (!str.empty()) vertexShader = Shader::AddVS(str);

	str = data.ReadWstring();
	if (!str.empty()) pixelShader = Shader::AddPS(str);

	str = data.ReadWstring();
	if (!str.empty()) SetDiffuseMap(str);

	str = data.ReadWstring();
	if (!str.empty()) SetSpecularMap(str);

	str = data.ReadWstring();
	if (!str.empty()) SetNormalMap(str);

	buffer->data.diffuse   = data.ReadVector4();
	buffer->data.specular  = data.ReadVector4();
	buffer->data.ambient   = data.ReadVector4();
	buffer->data.emissive  = data.ReadVector4();
	buffer->data.shininess = data.ReadFloat();
}

void Material::SaveDialog(string baseDir)
{
	if (ImGui::Button("SaveMaterial"))
		DIALOG->OpenDialog("SaveMaterial", "SaveMaterial", ".mat", baseDir);

	if (DIALOG->Display("SaveMaterial", 32, ImVec2(300, 100)))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			file = file.substr(projectDir.size() + 1, file.size());

			SaveTransform(ToWString(file));
		}

		DIALOG->Close();
	}
}

void Material::LoadDialog(string baseDir)
{
	if (ImGui::Button("LoadMaterial"))
		DIALOG->OpenDialog("LoadMaterial", "LoadMaterial", ".mat", baseDir);

	if (DIALOG->Display("LoadMaterial", 32, ImVec2(300, 100)))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			file = file.substr(projectDir.size() + 1, file.size());

			LoadTransform(ToWString(file));
		}

		DIALOG->Close();
	}
}
