#pragma once

template <typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if (!instance) instance = new T();

		return instance;
	}
	
	static void Delete()
	{
		if (instance) delete instance;
	}

protected:
	static T* instance;
};

template <typename T>
T* Singleton<T>::instance = nullptr;