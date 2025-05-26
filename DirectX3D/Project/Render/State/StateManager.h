#pragma once
class StateManager : public Singleton<StateManager>
{

	friend class Singleton;

private:

	enum RsStateMode
	{
		FILL_MODE_SOLID, // default
		FILL_MODE_WIREFRAME,
		FRONT_COUNTERCLOCKWISE,
		CULL_NONE
	};

	enum BlendStateMode
	{
		DISABLE_ALPHA,
		ENABLE_ALPHA,
		ALPHA_TO_COVERAGE,
		ADDITIVE_BLENDING
	};

	enum DepthStencilMode
	{
		DEPTH_ENABLED,
		DEPTH_DISABLED,
		DEPTH_WRITE_MASKZERO
	};

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
	void EnableAlphaToCoverage();
	void EnableAdditive();
	void DisableAlpha();

	void EnableDepth();
	void DisableDepth();

	void SetFrontCounterClockWise();
	void SetFrontClockWise();

	void DepthWriteMaskZero();

	void EnableBackFaceCulling();
	void DisableBackFaceCulling();

private:
	SamplerState* sampler{};

	vector<RasterizerState*> rs{};

	vector<BlendState*> blendStates{};

	vector<DepthStencilState*> depthStencilStates{};
};
