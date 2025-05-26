#pragma once

template<typename T>
class GameObject : public Transform
{
public:
	GameObject(wstring shaderFile);
	virtual ~GameObject();

	virtual void Render(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	virtual void RenderInstanced(UINT instanceCount, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	Material* GetMaterial() const { return material; }

protected:
	virtual void CreateMesh() = 0;

protected:
	vector<T>		vertices{};
	vector<UINT>	indices{};

	Mesh*			mesh{};
	Material*		material{};
};

template<typename T>
inline GameObject<T>::GameObject(wstring shaderFile)
{
	material = new Material(shaderFile);
}

template<typename T>
inline GameObject<T>::~GameObject()
{

	delete mesh;
	delete material;
}

template<typename T>
inline void GameObject<T>::Render(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	Transform::SetWorld();

	mesh->IASet(topology);
	material->Set();


	// Draw Call 이전까지는 setting -> setting한 결과를 가지고 draw call로 비로소 그려짐
	//DC->Draw(vertices.size(), 0);
	if (indices.size() > 0)
		DC->DrawIndexed(indices.size(), 0, 0); // Draw call
	else
		DC->Draw(vertices.size(), 0);
}

template<typename T>
inline void GameObject<T>::RenderInstanced(UINT instanceCount, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	Transform::SetWorld();

	mesh->IASet(topology);
	material->Set();

	if (indices.size() > 0) 
		DC->DrawIndexedInstanced(indices.size(), instanceCount, 0, 0, 0);
	else					
		DC->DrawInstanced(vertices.size(), instanceCount, 0, 0);
}

