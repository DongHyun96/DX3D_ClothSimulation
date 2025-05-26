#include "Framework.h"
#include "SceneManager.h"


SceneManager::SceneManager()
{
	InitDefaultFloor();
}

SceneManager::~SceneManager()
{
	for (pair<string, Scene*> scene : scenes)
		delete scene.second;

	scenes.clear();

	delete defaultFloor;
}

void SceneManager::Update()
{
	for (Scene* scene : curScenes)
		scene->Update();
}

void SceneManager::PreRender()
{
	for (Scene* scene : curScenes)
		scene->PreRender();
}

void SceneManager::Render()
{
	for (Scene* scene : curScenes)
		scene->Render();
}

void SceneManager::PostRender()
{
	for (Scene* scene : curScenes)
		scene->PostRender();
}

void SceneManager::Create(string key, Scene* scene)
{
	if (scenes.count(key) > 0) return;

	scenes.emplace(key, scene);
}

Scene* SceneManager::Add(string key)
{
	if (scenes.count(key) == 0) return nullptr;
		
	for (Scene* scene : curScenes)
	{
		if (scenes[key] == scene)
			return nullptr;
	}

	curScenes.push_back(scenes[key]);
	curScenes.back()->Initialize();

	return scenes[key];
}

void SceneManager::Remove(string key)
{
	if (scenes.count(key) == 0) return;

	for (UINT i = 0; i < curScenes.size(); i++)
	{
		if (scenes[key] == curScenes[i])
		{
			scenes[key]->Release();

			curScenes.erase(curScenes.begin() + i);
			return;
		}
	}
}

void SceneManager::InitDefaultFloor()
{
	defaultFloor = new TextureCube;
	defaultFloor->SetName("SpringMassFloor");
	defaultFloor->LoadTransform();

	defaultFloor->SetShader(L"16_Light");
	defaultFloor->SetDiffuseMap(L"Default/TemplateGrid_albedo.png");
}

