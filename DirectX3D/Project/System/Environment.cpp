#include "Framework.h"
#include "Environment.h"


Environment::Environment()
{
	CreatePerspective();
	CreateOrthoGraphic();

	mainCamera = new Camera();

	lightBuffer = new LightBuffer;
}

Environment::~Environment()
{
	delete lightBuffer;

	delete mainCamera;

	delete persBuffer;

	delete UIViewBuffer;
	delete orthoBuffer;
}

void Environment::CreatePerspective()
{
	persBuffer = new MatrixBuffer();

	persMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, WIN_WIDTH / WIN_HEIGHT, 0.1f, 1000.f);
}

void Environment::CreateOrthoGraphic()
{
	orthoMatrix = XMMatrixOrthographicOffCenterLH
	(
		0.f, WIN_WIDTH,
		0.f, WIN_HEIGHT,
		0.f, 1.f
	);

	orthoBuffer = new MatrixBuffer();
	orthoBuffer->SetData(orthoMatrix);

	UIViewBuffer = new ViewBuffer();
}

void Environment::Update()
{
	mainCamera->Update();
}

void Environment::Set()
{
	mainCamera->Set();

	persBuffer->SetData(persMatrix);
	persBuffer->SetVSBuffer(2);

	lightBuffer->SetPSBuffer(0);
}

void Environment::PostSet()
{
	UIViewBuffer->SetVSBuffer(1); // view
	orthoBuffer->SetVSBuffer(2);  // projection

	// TODO : ALPHA BLEND
}

void Environment::Debug()
{
	mainCamera->Debug();

	if (ImGui::TreeNode("Light Option"))
	{
		ImGui::DragFloat3("Direction", (float*)&lightBuffer->data.direction, 0.1f, -1.f, 1.f);

		ImGui::TreePop();
	}
}
