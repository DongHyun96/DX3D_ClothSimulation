#pragma once

class Camera : public Transform
{
public:
	Camera();
	~Camera();

	void Update();
	void Set();

	void Debug();

	void FreeMode();
	void TargetMode();

	void SaveData();
	void LoadData();

	struct Ray ScreenPointToRay(Vector3 screenPoint);

	Vector3 WorldToScreenPos(Vector3 worldPos);

	void SetTarget(Transform* target) { this->target = target; }

private:
	Matrix			viewMatrix{};
	ViewBuffer*		viewBuffer{};

	float moveSpeed{ 5.f };
	//float rotateSpeed{ 20.f };
	float rotateSpeed = 0.01f;


	Vector3 oldPos{};

	////////////////////////////////////////////////////////

	Transform* target{};

	float distance		= 20.f;
	float height		= 0.f;

	float moveDamping	= 15.f;
	float rotDamping	= 30.f;

	float rotY			= 0.f;
	float destRotY		= 0.f;
	float destRotX		= 0.f;


	Vector3 destPos{};
	Vector3 focusOffset{};

	Matrix rotMatrix{};

};