#include "Framework.h"
#include "Transform.h"

UINT Transform::index = 0;

Transform::Transform()
{
	worldBuffer = new MatrixBuffer;
	world = XMMatrixIdentity();

	name = "Object" + to_string(index++);
}

Transform::~Transform()
{
	delete worldBuffer;

}

void Transform::SetWorld()
{
	worldBuffer->SetData(world);
	worldBuffer->SetVSBuffer(0);
}

void Transform::Update()
{
	UpdateWorldMatrix();
}

void Transform::Debug()
{

	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::Checkbox("IsActive", &isActive);

		ImGui::DragFloat3("Scale",			(float*)&scale,			0.01f, 0.01f, 100.f);
		//ImGui::DragFloat3("Rotation",		(float*)&rotation,		0.01f, 0.01f, 100.f);
		ImGui::SliderAngle("RotationX", &rotation.x);
		ImGui::SliderAngle("RotationY", &rotation.y);
		ImGui::SliderAngle("RotationZ", &rotation.z);

		ImGui::DragFloat3("Translation",	(float*)&translation,	1.00f, -2000.f, 200.f);

		if (ImGui::Button("Save")) Save();
		if (ImGui::Button("Load")) Load();

		ImGui::TreePop();
	}
}

void Transform::Save()
{
	BinaryWriter data("_TextData/Transform/" + name + ".transform");

	data.WriteData(scale);
	data.WriteData(rotation);
	data.WriteData(translation);
	data.WriteData(pivot);
}

void Transform::Load()
{
	BinaryReader data("_TextData/Transform/" + name + ".transform");

	if (!data.Succeeded()) return;

	scale		= data.ReadVector3();
	rotation	= data.ReadVector3();
	translation = data.ReadVector3();
	pivot		= data.ReadVector3();

}

void Transform::UpdateWorldMatrix()
{
	Matrix S	= XMMatrixScaling(scale.x, scale.y, scale.z);
	Matrix R	= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	Matrix T	= XMMatrixTranslation(translation.x, translation.y, translation.z);

	Matrix P	= XMMatrixTranslation(pivot.x, pivot.y, pivot.z);
	Matrix IP	= XMMatrixInverse(nullptr, P);

	world = IP * S * R * T * P;

	if (parent) world *= parent->world;

	XMFLOAT4X4 fWorld;

	XMStoreFloat4x4(&fWorld, world);
	
	right	= Vector3(fWorld._11, fWorld._12, fWorld._13).GetNormalized();
	up		= Vector3(fWorld._21, fWorld._22, fWorld._23).GetNormalized();
	forward = Vector3(fWorld._31, fWorld._32, fWorld._33).GetNormalized();

	// Global Transform 구하기

	XMVECTOR outS, outR, outT;

	XMMatrixDecompose(&outS, &outR, &outT, world);

	globalScale		= outS;
	globalRotation	= outR; // 주의 outR은 Quaternion의 값 (오일러 각이 아님)
	globalPosition	= outT;

}
