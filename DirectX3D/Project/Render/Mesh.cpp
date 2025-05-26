#include "Framework.h"
#include "Mesh.h"


Mesh::~Mesh()
{
	delete vertexBuffer;

	if (indexBuffer) delete indexBuffer;
}

void Mesh::IASet(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	vertexBuffer->IASetBuffer(topology);

	if (indexBuffer) indexBuffer->IASetBuffer();
}

