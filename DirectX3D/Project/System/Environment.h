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

	Matrix GetPersMatrix() const { return persMatrix; }
	
	Camera* GetMainCamera() const { return mainCamera; }

private:
	Matrix			persMatrix{};
	MatrixBuffer*	persBuffer{};

	Matrix			orthoMatrix{};
	MatrixBuffer*	orthoBuffer{};

	ViewBuffer*		UIViewBuffer{};

	Camera*			mainCamera{};

	LightBuffer*	lightBuffer{};
};