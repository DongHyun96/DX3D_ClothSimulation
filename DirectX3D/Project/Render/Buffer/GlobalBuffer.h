#pragma once

struct MatrixBuffer : public ConstBuffer<Matrix>
{
	MatrixBuffer()
		:ConstBuffer(matrix)
	{
		matrix = XMMatrixIdentity();
	}

	void SetData(Matrix value)
	{
		matrix = XMMatrixTranspose(value);
	}

	Matrix matrix{};
};

struct ViewData
{
	Matrix view;
	Matrix invView;
};

//ViewBuffer
struct ViewBuffer : public ConstBuffer<ViewData>
{
	ViewBuffer()
		:ConstBuffer(data)
	{
		data.view		= XMMatrixIdentity();
		data.invView	= XMMatrixIdentity();
	}

	void SetData(Matrix view, Matrix invView)
	{
		data.view		= XMMatrixTranspose(view);
		data.invView	= XMMatrixTranspose(invView);
	}

	ViewData data{};
};


struct LightData
{
	Vector3 direction = Vector3(0, -1, 0);

	int type{}; // Directional, pointLight, spotLight ...type

	Vector4 color = Vector4(1, 1, 1, 1);

	// Point Light
	Vector3 position = Vector3(0, 100, 0);
	float   range    = 100.f;

	// Spot Light
	float innerRange = 55.f; // Angle
	float outerRange = 70.f; // Angle

	// Capsule Light
	float length = 50.f; // height
	
	int active = true;
};

#define MAX_LIGHT 10

struct Lights
{
	LightData lights[MAX_LIGHT];

	int lightCount			= 1;
	Vector3 ambientLight	= Vector3(0.1f, 0.1f, 0.1f);
	Vector3 ambientCeil		= Vector3(0.1f, 0.1f, 0.1f); // 제한 밝기
	float padding{};

};


struct LightBuffer : public ConstBuffer<Lights>
{
	LightBuffer()
		:ConstBuffer(data)
	{
	}

	Lights data{};
};

struct RayData
{
	Vector3 origin{};
	UINT	polygonCount{};

	Vector3 direction{};
	float	padding{};
};

struct RayBuffer : public ConstBuffer<RayData>
{
	RayBuffer()
		:ConstBuffer(data)
	{
	}

	RayData data{};
};

struct BrushData
{
	int		type{};
	Vector3 pickedPos{};

	float	range = 10.f;
	Vector3 color = { 0, 1, 0 };
};

struct BrushBuffer : public ConstBuffer<BrushData>
{
	BrushBuffer()
		:ConstBuffer(data)
	{
	}

	BrushData data{};
};

struct MaterialData
{
	Vector4 diffuse  = Vector4(1, 1, 1, 1);
	Vector4 specular = Vector4(0, 0, 0, 1);
	Vector4 ambient  = Vector4(0.1f, 0.1f, 0.1f, 1.f);
	Vector4 emissive = Vector4(0, 0, 0, 0); //// Material 자체가 빛을 내는 요소 (무드등같은 느낌)

	float shininess = 24.f;

	int hasDiffuseMap	= false;
	int hasSpecularMap	= false;
	int hasNormalMap	= false;
};

struct MaterialBuffer : public ConstBuffer<MaterialData>
{
	MaterialBuffer()
		:ConstBuffer(data)
	{
	}

	MaterialData data{};
};

struct FloatValueData
{
	float values[4]{};
};

struct FloatValueBuffer : public ConstBuffer<FloatValueData>
{
	FloatValueBuffer()
		:ConstBuffer(data)
	{
	}

	FloatValueData data{};
};

struct IntValueData
{
	int values[4]{};
};

struct IntValueBuffer : public ConstBuffer<IntValueData>
{
	IntValueBuffer()
		:ConstBuffer(data)
	{
	}

	IntValueData data{};
};