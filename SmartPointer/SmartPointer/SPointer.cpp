#include <iostream>
#include "SPointer.h"

template<class T>
void SPointer<T>::initialize(T* objPointer, size_t* initCounter)
{
	pointer = objPointer;
	counter = initCounter;
}

template<class T>
SPointer<T>::SPointer(T* objPointer)
{
	initialize(objPointer, new size_t(1));
}

template<class T>
SPointer<T>::SPointer()
{
	counter = new size_t();
}

template<class T>
SPointer<T>::SPointer(const SPointer& other)
{
	counter = new size_t();
	initialize(other.pointer, other.counter);
}

template<class T>
SPointer<T>::~SPointer()
{
	Release();
}

template<class T>
T* SPointer<T>::Get()
{
	return pointer;
}

template<class T>
void SPointer<T>::Set(T* objPointer)
{
	Release();

	initialize(objPointer, new size_t(1));
}

template<class T>
void SPointer<T>::Release()
{
	if (pointer == nullptr)
	{
		return;
	}

	(*counter)--;
	if (*counter == 0)
	{
		delete pointer;
		delete counter;
	}
}

template<class T>
SPointer<T>& SPointer<T>::operator=(const SPointer<T>& other)
{
	Release();

	initialize(other.pointer, other.counter);
	(*counter)++;
	return *this;
}

template<class T>
T* SPointer<T>::operator->()
{
	return pointer;
}

template<class T>
T SPointer<T>::operator*()
{
	return *pointer;
}

class A
{
public:
	int a;
};

int foo(SPointer<A> a)
{
	return a->a * 17 + 2;
}

int main()
{
	SPointer<A> nonum;
	SPointer<A> num(new A());
	nonum = num;
	num->a = 1;

	std::cout << num->a << "\n";
	std::cout << (*num).a << "\n";
	num->a = foo(num);
	std::cout << nonum->a << "\n";
}