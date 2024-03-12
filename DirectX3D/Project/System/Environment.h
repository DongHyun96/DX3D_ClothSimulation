#pragma once

class Environment : public Singleton<Environment>
{

	friend class Singleton;

private:
	Environment();
	~Environment();

	void CreatePerspective();
	void CreateOrthoGraphic();

public:

	void Update();
	void Set();

	void PostSet();

	void Debug();
	void DebugLight(int lightIndex);

	Matrix GetPersMatrix() const		{ return persMatrix; }
	Camera* GetMainCamera()	const		{ return mainCamera; }
	LightBuffer* GetLightBuffer() const { return lightBuffer; }

private:
	Matrix			persMatrix{};
	MatrixBuffer*	persBuffer{};

	Matrix			orthoMatrix{};
	MatrixBuffer*	orthoBuffer{};

	ViewBuffer*		UIViewBuffer{};

	Camera*			mainCamera{};

	LightBuffer*	lightBuffer{};
};
