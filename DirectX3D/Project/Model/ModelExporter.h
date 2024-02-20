#pragma once

class ModelExporter
{
public:
	ModelExporter(string name);
	~ModelExporter();

	void ExportModel();
	void ExportAnimation(string file); 

private:
	void ExportMaterial();
	void ExportMesh();

	void ReadMaterial();
	void WriteMaterial();

	void ReadMesh(aiNode* node);
	void WriteMesh();

	// Animation 관련
	void ReadNode(aiNode* node, int index, int parent);
	void ReadBone(aiMesh* mesh, OUT vector<VertexWeights>& vertexWeights);

	Clip* ReadClip(aiAnimation* animation);
	void WriteClip(Clip* clip, string file);

	// 모든 bone에 모든 frame별 transform(keyFrame)을 읽음
	void ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes);

	////////////////////////////////////////////////////////////////////////////

	wstring CreateTexture(string file);

private:

	string name{};

	Assimp::Importer* importer{};

	const aiScene* scene{};

	vector<Material*> materials{};

	vector<MeshData*> meshes{};

	// Animation

	vector<NodeData*> nodes{};
	vector<BoneData*> bones{};

	map<string, UINT> boneMap{};

	UINT boneCount{};
};
