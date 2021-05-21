#pragma once

#include <cassert>

template<typename T>
class LHSingleton
{
private:
	static T* ms_singleton;

public:
	LHSingleton(void)
	{
		assert(!ms_singleton);
		int offset = (int)(T*)1 - (int)(LHSingleton<T>*)(T*)1;
		ms_singleton = (T*)((int)this+offset);
	}
	virtual ~LHSingleton(void)
	{
		assert(ms_singleton);
		ms_singleton = 0;
	}

	static T& GetSingleton()
	{
		assert(ms_singleton);
		return (*ms_singleton);
	}
	static T* GetSingletonPtr()
	{
		assert(ms_singleton);
		return (ms_singleton);
	}
};

template<typename T> T* LHSingleton<T>::ms_singleton = 0;