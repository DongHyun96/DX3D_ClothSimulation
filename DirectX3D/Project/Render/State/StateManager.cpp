#include "Framework.h"
#include "StateManager.h"


StateManager::StateManager()
{
	CreateSampler();
	CreateRasterizer();
	CreateBlendState();
	CreateDepthStencilState();
}

StateManager::~StateManager()
{
	delete sampler;

	for (RasterizerState* state : rs)
		delete state;

	rs.clear();

	for (BlendState* state : blendStates)
		delete state;

	blendStates.clear();

	for (DepthStencilState* state : depthStencilStates)
		delete state;

	depthStencilStates.clear();
}

void StateManager::CreateSampler()
{
	sampler = new SamplerState();
}

void StateManager::CreateRasterizer()
{
	rs.emplace_back(new RasterizerState);
	rs.emplace_back(new RasterizerState);

	rs[0]->SetFillMode(D3D11_FILL_SOLID);
	rs[1]->SetFillMode(D3D11_FILL_WIREFRAME);

	rs[0]->CreateState();
	rs[1]->CreateState();

	rs.emplace_back(new RasterizerState);
	rs[2]->SetFrontCounterClockWise(true);
	rs[2]->CreateState();
}

void StateManager::CreateBlendState()
{
	blendStates.emplace_back(new BlendState);
	blendStates.emplace_back(new BlendState);

	blendStates[0]->SetAlpha(false);
	blendStates[0]->CreateState();

	blendStates[1]->SetAlpha(true);
	blendStates[1]->CreateState();
}

void StateManager::CreateDepthStencilState()
{
	depthStencilStates.emplace_back(new DepthStencilState);
	depthStencilStates.emplace_back(new DepthStencilState);
	depthStencilStates.emplace_back(new DepthStencilState);

	depthStencilStates[1]->DepthEnable(false);
	depthStencilStates[2]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);

}

void StateManager::EnableWireFrame()
{
	rs[1]->SetState();
}

void StateManager::DisableWireFrame()
{
	rs[0]->SetState();
}

void StateManager::EnableAlpha()
{
	blendStates[1]->SetState();
}

void StateManager::DisableAlpha()
{
	blendStates[0]->SetState();
}

void StateManager::EnableDepth()
{
	depthStencilStates[0]->SetState();
}

void StateManager::DisableDepth()
{
	depthStencilStates[1]->SetState();
}

void StateManager::SetFrontCounterClockWise()
{
	rs[2]->SetState();
}

void StateManager::SetFrontClockWise()
{
	rs[0]->SetState();
}
