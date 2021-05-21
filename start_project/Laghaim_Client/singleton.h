#ifndef		__SINGLETON_H__
#define		__SINGLETON_H__

template <class T>
class CSingleton
{
private:
protected:
	// must protected
	CSingleton() {}
	~CSingleton() {}

public:
	static T*	GetInstance();
};

template <class T>
T* CSingleton<T>::GetInstance()
{
	static	T	m_Instance;
	return	&m_Instance;
}


#endif