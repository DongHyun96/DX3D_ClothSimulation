#pragma once
class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton;

private:
	SceneManager();
	~SceneManager();

public:
	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void Create(string key, Scene* scene);

	Scene* Add(string key);
	void Remove(string key);

private:
	map<string, Scene*> scenes{};

	vector<Scene*> curScenes{};
};
