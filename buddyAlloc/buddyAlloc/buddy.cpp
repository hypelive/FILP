#include "buddy.h"

Buddy::Buddy(size_t size, void* data)
{
	this->size = size;
	this->data = data;
}

bool Buddy::isEmpty()
{
	return data == nullptr;
}
