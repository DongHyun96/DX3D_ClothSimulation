#pragma once

typedef VertexTextureNormalTangentBlend ModelVertex;

struct MeshData
{
	string name{};
	UINT materialIndex{};

	vector<ModelVertex> vertices{};
	vector<UINT>		indices{};
};

struct NodeData
{
	int		index = -1;
	string	name{};
	int		parent = -1; // parent idx
	Matrix	transform = XMMatrixIdentity();
};

struct BoneData
{
	int		index = -1;
	string	name{};
	Matrix	offset = XMMatrixIdentity();
};

struct VertexWeights
{
	void AddData(const UINT& index, const float& weight)
	{
		for (UINT i = 0; i < 4; i++)
		{
			if (indices[i] == -1)
			{
				indices[i] = index;
				weights[i] = weight;

				return;
			}
		}
	}

	void Normalize()
	{
		float sum = 0.f;

		for (UINT i = 0; i < 4; i++)
			sum += weights[i];

		if (sum == 0) return;

		for (UINT i = 0; i < 4; i++)
			weights[i] /= sum;
	}

	int		indices[4] = {-1, -1, -1, -1};
	float	weights[4] = {};
};

struct KeyTransform
{
	Vector3 scale{};
	Vector4 rotation{};   // quaternion
	Vector3 translation{};
};

// 1차원 배열 data | 특정 bone에 해당되는 keyFrame
struct KeyFrame
{
	string boneName{};

	vector<KeyTransform> transforms{};
};

struct ClipNode
{
	aiString name{};

	vector<KeyTransform> transforms{};
};

struct Clip
{
	string name{};

	UINT frameCount{};

	float ticksPerSecond{};

	vector<KeyFrame*> keyFrame{}; // 2차원 배열
};

struct ClipTransform
{
	Matrix transform[MAX_FRAME][MAX_BONE];
};
