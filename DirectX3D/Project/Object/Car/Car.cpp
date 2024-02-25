#include "Framework.h"
#include "Car.h"


Car::Car(string carName)
{
	this->name = carName + " MainTransform";

	mainModel = new Model(carName);
	mainModel->SetParent(this);

	string wheelName = carName + "Wheel";

	wheels = new ModelInstancing(wheelName);
	wheels->LoadTransforms();
	wheels->SetParent(this);

	this->LoadTransform();
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
