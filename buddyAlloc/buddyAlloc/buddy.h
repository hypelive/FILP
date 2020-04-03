#pragma once
struct Buddy
{
	size_t size;
	void* data;

	Buddy(size_t size, void* data);
	bool isEmpty();
};

