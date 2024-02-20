#pragma once
class BlendState
{
public:
	BlendState();
	~BlendState();

	void SetState();
	
	void CreateState();

	void SetAlpha(bool isAlpha);
	void SetAlphaToCoverage(bool isAlphaToCoverage);
	void SetAdditive();

private:

	D3D11_BLEND_DESC	desc{};
	ID3D11BlendState*	state{};
};
