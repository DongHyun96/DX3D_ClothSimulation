#pragma once
class StateManager : public Singleton<StateManager>
{

	friend class Singleton;

private:
	StateManager();
	~StateManager();

	void CreateSampler();
	void CreateRasterizer();
	void CreateBlendState();
	void CreateDepthStencilState();

public:

	void EnableWireFrame();
	void DisableWireFrame();

	void EnableAlpha();
	void DisableAlpha();

	void EnableDepth();
	void DisableDepth();

	void SetFrontCounterClockWise();
	void SetFrontClockWise();

private:
	SamplerState* sampler{};

	vector<RasterizerState*> rs{};

	vector<BlendState*> blendStates{};

	vector<DepthStencilState*> depthStencilStates{};
};
