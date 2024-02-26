#include "Framework.h"
#include "Model.h"


Model::Model(string name)
{
	this->name = name;

	ReadMaterial();
	ReadMesh();

	Transform::LoadTransform();
}

Model::~Model()
{
	for (Material* material : materials)
		delete material;

	materials.clear();

	for (ModelMesh* mesh : meshes)
		delete mesh;

	meshes.clear();

	for (pair<string, Transform*> pair : sockets)
		delete pair.second;

	sockets.clear();
}

void Model::Update()
{
	Transform::Update();

	UpdateSockets();
}

void Model::Render(bool hasAnimation)
{
	Transform::SetWorld(hasAnimation);

	for (ModelMesh* mesh : meshes)
	{
		materials[mesh->GetMaterialIndex()]->Set();

		mesh->Render();
	}
}

void Model::RenderInstanced(UINT instanceCount)
{
	Transform::SetWorld();

	for (ModelMesh* mesh : meshes)
	{
		materials[mesh->GetMaterialIndex()]->Set();

		mesh->RenderInstanced(instanceCount);
	}
}

void Model::Debug()
{
	Transform::Debug();

	for (Material* material : materials)
		material->Debug();

	if (ImGui::Button(("Save Model" + name).c_str()))
		SaveModel();

	if (ImGui::Button(("Load Model" + name).c_str()))
	{
		Transform::LoadTransform();

		for (Material* material : materials)
			material->LoadTransform(L"_ModelData/Material/" + ToWString(name + "/" + material->GetName()) + L".mat");
	}
}

void Model::SetShader(wstring file)
{
	for (Material* material : materials)
		material->SetShader(file);
}

void Model::AttachToBone(ModelAnimator* model, string boneName)
{
	if (model->sockets.count(boneName) > 0)
	{
		this->SetParent(model->sockets[boneName]);
		return;
	}

	Transform* socket = new Transform; // 오른손

	this->SetParent(socket); // Sword의 parent를 socket으로 둠

	model->sockets.emplace(boneName, socket);
}

void Model::SaveModel()
{
	Transform::SaveTransform();

	for (Material* material : materials)
		material->SaveTransform(L"_ModelData/Material/" + ToWString(name + "/" + material->GetName()) + L".mat");
}

void Model::ReadMaterial()
{
	string path = "_ModelData/Material/" + name + "/MaterialList.list";

	BinaryReader data(ToWString(path));

	UINT size = data.ReadUint();

	for (UINT i = 0; i < size; i++)
	{
		Material* material = new Material(L"05_NormalMapping");

		string file = data.ReadString();

		material->LoadTransform(ToWString(file));

		materials.push_back(material);
	}
}

void Model::ReadMesh()
{
	string path = "_ModelData/Mesh/" + name + ".mesh";

	BinaryReader data(ToWString(path));

	UINT size = data.ReadUint();

	for (UINT i = 0; i < size; i++)
	{
		string name = data.ReadString();

		UINT materialIndex = data.ReadUint();

		vector<ModelVertex> vertices{};
		vector<UINT>		indices{};

		{
			UINT count = data.ReadUint();

			vertices.resize(count);

			void* ptr = vertices.data();

			data.ReadData(&ptr, sizeof(ModelVertex) * count);
		}
		{
			UINT count = data.ReadUint();

			indices.resize(count);

			void* ptr = indices.data();

			data.ReadData(&ptr, sizeof(UINT) * count);
		}

		ModelMesh* mesh = new ModelMesh(name, materialIndex, vertices, indices);

		meshes.push_back(mesh);
	}

	size = data.ReadUint();

	for (UINT i = 0; i < size; i++)
	{
		NodeData node;

		node.index		= data.ReadInt();
		node.name		= data.ReadString();
		node.parent		= data.ReadInt();
		node.transform	= data.ReadMatrix();

		nodes.emplace_back(node);
	}

	size = data.ReadUint();

	for (UINT i = 0; i < size; i++)
	{
		BoneData bone;

		bone.index		= data.ReadInt();
		bone.name		= data.ReadString();
		bone.offset		= data.ReadMatrix();

		bones.emplace_back(bone);

		boneMap.emplace(bone.name, bone.index);
	}
}

void Model::UpdateSockets()
{
	// Sword는 socket에 붙어있고 해당 socket만 잘 이동 시키면 됨
	for (pair<string, Transform*> pair : sockets)
	{
		Transform* socket		= pair.second;
		ModelAnimator* parent	= dynamic_cast<ModelAnimator*>(this);

		socket->GetWorld() = parent->GetTransformByBone(pair.first) * this->world;
	}
}
