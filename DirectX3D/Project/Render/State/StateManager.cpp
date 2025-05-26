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

	rs[FILL_MODE_SOLID]->SetFillMode(D3D11_FILL_SOLID);
	rs[FILL_MODE_WIREFRAME]->SetFillMode(D3D11_FILL_WIREFRAME);

	rs[FILL_MODE_SOLID]->CreateState();
	rs[FILL_MODE_WIREFRAME]->CreateState();

	rs.emplace_back(new RasterizerState);
	rs[FRONT_COUNTERCLOCKWISE]->SetFrontCounterClockWise(true);
	rs[FRONT_COUNTERCLOCKWISE]->CreateState();

	rs.emplace_back(new RasterizerState);
	rs[CULL_NONE]->SetCullMode(D3D11_CULL_NONE);
	rs[CULL_NONE]->CreateState();
}

void StateManager::CreateBlendState()
{
	blendStates.emplace_back(new BlendState);
	blendStates.emplace_back(new BlendState);

	blendStates[DISABLE_ALPHA]->SetAlpha(false);

	blendStates[ENABLE_ALPHA]->SetAlpha(true);

	// AlphaToCoverage
	blendStates.emplace_back(new BlendState);
	blendStates[ALPHA_TO_COVERAGE]->SetAlphaToCoverage(true);

	// Additive
	blendStates.emplace_back(new BlendState);
	blendStates[ADDITIVE_BLENDING]->SetAdditive();
	//blendStates[3]->SetAlphaToCoverage(true);
}

void StateManager::CreateDepthStencilState()
{
	depthStencilStates.emplace_back(new DepthStencilState);
	depthStencilStates.emplace_back(new DepthStencilState);
	depthStencilStates.emplace_back(new DepthStencilState);

	depthStencilStates[DEPTH_DISABLED]->DepthEnable(false);
	depthStencilStates[DEPTH_WRITE_MASKZERO]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);

}

void StateManager::EnableWireFrame()
{
	rs[FILL_MODE_WIREFRAME]->SetState();
}

void StateManager::DisableWireFrame()
{
	rs[FILL_MODE_SOLID]->SetState();
}

void StateManager::EnableAlpha()
{
	blendStates[ENABLE_ALPHA]->SetState();
}

void StateManager::EnableAlphaToCoverage()
{
	blendStates[ALPHA_TO_COVERAGE]->SetState();
}

void StateManager::EnableAdditive()
{
	blendStates[ADDITIVE_BLENDING]->SetState();
}

void StateManager::DisableAlpha()
{
	blendStates[DISABLE_ALPHA]->SetState();
}

void StateManager::EnableDepth()
{
	depthStencilStates[DEPTH_ENABLED]->SetState();
}

void StateManager::DisableDepth()
{
	depthStencilStates[DEPTH_DISABLED]->SetState();
}

void StateManager::SetFrontCounterClockWise()
{
	rs[FRONT_COUNTERCLOCKWISE]->SetState();
}

void StateManager::SetFrontClockWise()
{
	rs[FILL_MODE_SOLID]->SetState();
}

void StateManager::DepthWriteMaskZero()
{
	depthStencilStates[DEPTH_WRITE_MASKZERO]->SetState();
}

void StateManager::EnableBackFaceCulling()
{
	rs[FILL_MODE_SOLID]->SetState();
}

void StateManager::DisableBackFaceCulling()
{
	rs[CULL_NONE]->SetState();
}
