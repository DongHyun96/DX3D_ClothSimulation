#pragma once
class Transform
{
public:
	Transform();
	virtual ~Transform();

	void SetWorld();

	virtual void Update();
	
	void Debug();

	void SetParent(Transform* parent)		{ this->parent = parent; }
	void SetPivot(const Vector3& pivot)		{ this->pivot = pivot; }

	Vector3 GetGlobalScale()	const { return globalScale; }
	Vector3 GetGlobalRotation() const { return globalRotation; }
	Vector3 GetGlobalPosition() const { return globalPosition; }
	
	Vector3 GetRightVector()	const { return right; }
	Vector3 GetUpVector()		const { return up; }
	Vector3 GetForwardVector()	const { return forward; }

	void SetName(string name) { this->name = name; }

	void Save();
	void Load();

	Matrix& GetWorld() { return world; }

	Transform* GetParent() const { return parent; }

	bool GetIsActive() { return isActive; }

	void SetIsActive(bool isActive) { this->isActive = isActive; }

private:
	void UpdateWorldMatrix();

public:

	Vector3 scale{1.f, 1.f, 1.f};
	Vector3 rotation{};
	Vector3 translation{};

protected:

	Matrix			world;
	MatrixBuffer*	worldBuffer{};

	Transform* parent{};

	Vector3 right{}, up{}, forward{};

	Vector3 globalScale{};
	Vector3 globalRotation{};
	Vector3 globalPosition{};

	Vector3 pivot{};

	string name{}; // label

	static UINT index;

	bool isActive = true;
};
