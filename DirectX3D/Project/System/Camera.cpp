#include "Framework.h"
#include "Camera.h"

Camera::Camera()
{
	viewBuffer = new ViewBuffer();
	translation.z = -5.f;

	LoadData();
}

Camera::~Camera()
{
	SaveData();

	delete viewBuffer;
}

void Camera::Update()
{
	//XMVECTOR eye   = XMVectorSet(+0, +0, -3, 0);
	//XMVECTOR focus = XMVectorSet(+0, +0, +0, 0);
	//XMVECTOR up    = XMVectorSet(+0, +1, +0, 0);
	//               
	//viewMatrix     = XMMatrixLookAtLH(eye, focus, up); // ī�޶� �ٶ󺸴� ������ �����ؼ� view mat�� ����� ��

	Transform::Update();

	if (target) TargetMode();
	else		FreeMode();
}

void Camera::Set()
{
	if (!target)
		viewMatrix = XMMatrixInverse(nullptr, world);

	viewBuffer->SetData(viewMatrix, world);
	viewBuffer->SetVSBuffer(1);
}

void Camera::Debug()
{
	ImGuiIO io = ImGui::GetIO();

	distance -= io.MouseWheel * moveSpeed;

	distance = Clamp(distance, 10, 50);

	if (ImGui::TreeNode("Camera Option"))
	{
		ImGui::Text("Pos : %d %d %d\n", (int)translation.x, (int)translation.y, (int)translation.z);
		ImGui::Text("Rot : %.2f %.2f %.2f\n", rotation.x, rotation.y, rotation.z);

		ImGui::SliderFloat("Move Speed", &moveSpeed, 1.f, 150.f);
		ImGui::SliderFloat("Rotate Speed", &rotateSpeed, 0.01f, 150.f);
		
		ImGui::SliderFloat("Height",   &height,	  -10.f, 150.f);
		ImGui::SliderFloat("Distance", &distance, 0.f, 50.f);

		ImGui::SliderAngle("RotationY", &rotY);

		ImGui::SliderFloat("MoveDamping", &moveDamping, 0.f, 30.f);
		ImGui::SliderFloat("RotDamping", &rotDamping, 0.f, 30.f);


		if (ImGui::Button("Set Default"))
		{
			scale		= { 1, 1, 1 };
			rotation	= {0, 0, 0};
			translation = {0, 0, -10};
		}

		ImGui::TreePop();
	}
}

void Camera::FreeMode()
{
	if (KEY_PRESS(VK_RBUTTON))
	{
		if (KEY_PRESS('A'))
		{
			translation += -GetRightVector() * moveSpeed * DELTA_TIME;
		}
		if (KEY_PRESS('D'))
		{
			translation += +GetRightVector() * moveSpeed * DELTA_TIME;
		}
		if (KEY_PRESS('E'))
		{
			translation += -GetUpVector() * moveSpeed * DELTA_TIME;
		}
		if (KEY_PRESS('Q'))
		{
			translation += +GetUpVector() * moveSpeed * DELTA_TIME;
		}
		if (KEY_PRESS('S'))
		{
			translation += -GetForwardVector() * moveSpeed * DELTA_TIME;
		}
		if (KEY_PRESS('W'))
		{
			translation += +GetForwardVector() * moveSpeed * DELTA_TIME;
		}

		Vector3 delta = mousePos - oldPos;

		rotation.x += -delta.y * rotateSpeed;
		rotation.y += delta.x * rotateSpeed;

	}
	oldPos = mousePos;
}

void Camera::TargetMode()
{
	if (KEY_PRESS(VK_RBUTTON))
	{
		Vector3 delta = mousePos - oldPos;

		rotation.x += -delta.y * rotateSpeed;
		rotation.y += delta.x * rotateSpeed;
	}

	oldPos = mousePos;

	destRotY = LERP(destRotY, this->rotation.y, DELTA_TIME * rotDamping);
	destRotX = LERP(destRotX, this->rotation.x, DELTA_TIME * rotDamping);

	rotMatrix = XMMatrixRotationRollPitchYaw(destRotX, destRotY + rotY, 0.f);

	Vector3 backward = XMVector3TransformNormal(Vector3(0, 0, -1), rotMatrix);

	destPos  = target->GetGlobalPosition() + backward * distance + Vector3(0, 1, 0) * height;

	translation = LERP(translation, destPos, DELTA_TIME * moveDamping);

	Vector3 targetPos = target->GetGlobalPosition();

	targetPos.y += height;

	viewMatrix = XMMatrixLookAtLH(translation, targetPos, Vector3(0, 1, 0));

}

void Camera::SaveData()
{
	BinaryWriter data("_TextData/Camera.data");

	data.WriteData(scale);
	data.WriteData(rotation);
	data.WriteData(translation);

	data.WriteData(moveSpeed);
	data.WriteData(rotateSpeed);
}

void Camera::LoadData()
{
	BinaryReader data("_TextData/Camera.data");

	if (!data.Succeeded()) return;

	scale		= data.ReadVector3();
	rotation	= data.ReadVector3();
	translation = data.ReadVector3();

	moveSpeed	= data.ReadFloat();
	rotateSpeed = data.ReadFloat();
}

Ray Camera::ScreenPointToRay(Vector3 screenPoint)
{
	Vector3 screenSize(WIN_WIDTH, WIN_HEIGHT, 1.f);

	// Inv Viewport
	// ȭ�� point�� ndc��ǥ���
	Vector3 point;
	point.x = (screenPoint.x / screenSize.x) * 2.f - 1.f;
	point.y = (screenPoint.y / screenSize.y) * 2.f - 1.f;
	//point.y *= -1;
	point.z = 1.f;

	// Inv Projection
	
	//XMFLOAT4X4 projection;
	//XMStoreFloat4x4(&projection, ENVIRONMENT->GetPersMatrix());

	Matrix invProj = XMMatrixInverse(nullptr, ENVIRONMENT->GetPersMatrix());

	point = XMVector3TransformCoord(point, invProj);

	// Inv View(Cam)
	// view Mat�� world�� �����, ���� inv view�� world�� ��
	point = XMVector3TransformCoord(point, world);

	Ray ray;

	ray.origin		= globalPosition;
	ray.direction	= Vector3(point - globalPosition).GetNormalized();

	return ray;
}

Vector3 Camera::WorldToScreenPos(Vector3 worldPos)
{
	Vector3 screenPos{};

	screenPos = XMVector3TransformCoord(worldPos, viewMatrix);
	screenPos = XMVector3TransformCoord(screenPos, ENVIRONMENT->GetPersMatrix());
		
	screenPos += Vector3(1, 1, 1);
	screenPos *= 0.5f;

	screenPos.x *= WIN_WIDTH;
	screenPos.y *= WIN_HEIGHT;

	return screenPos;
}
