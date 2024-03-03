#include "Framework.h"
#include "Car.h"


Car::Car(string carName)
{
	this->name = carName + " MainTransform";
	this->LoadTransform();

	mainModel = new Model(carName);
	mainModel->SetParent(this);

	string wheelName = carName + "Wheel";

	wheels = new ModelInstancing(wheelName);
	wheels->LoadTransforms();
	wheels->SetParent(this);
}

Car::~Car()
{
	delete mainModel;

	delete wheels;
}

void Car::Update()
{
	Transform::Update();

	mainModel->Update();

	wheels->Update();
	//Move();

	if (KEY_PRESS(VK_UP))
		translation += forward * DELTA_TIME * 10.f;
	if (KEY_PRESS(VK_RIGHT))
		translation += right * DELTA_TIME * 10.f;
}

void Car::Render()
{
	mainModel->Render();

	wheels->Render();
}

void Car::Debug()
{
	Transform::Debug();

	mainModel->Debug();

	wheels->Debug();

	if (ImGui::Button(("Save Car " + mainModel->GetName()).c_str()))
	{
		Transform::SaveTransform();

		mainModel->SaveModel();
		
		wheels->SaveTransforms();
	}
}
