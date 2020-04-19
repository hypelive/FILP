#pragma once
template <class T>
class SPointer
{
private:
	T* pointer = nullptr;
	size_t* counter = nullptr;

	void initialize(T* objPointer, size_t* initCounter);
public:
	SPointer(T* objPointer);
	SPointer();
	SPointer(const SPointer& other);
	~SPointer();

	T* Get();
	void Set(T* objPointer);
	void Release();

	SPointer& operator=(const T& objPointer) = delete;
	SPointer& operator=(const SPointer& objPointer);
	T* operator->();
	T operator*();
};
