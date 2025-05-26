#pragma once

class Car : public Transform
{
public:
	Car(string carName);
	virtual ~Car();

	void Update();
	void Render();

	//virtual void Move() = 0;

	void Debug();

private:

	Model* mainModel{};

	// 0,1 Front 2,3 rear
	ModelInstancing* wheels{};

};
