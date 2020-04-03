#pragma once
#include <vector>
#include "buddy.h"

class BuddyAllocator
{
private:
	size_t maxSize;
	int levelsCount;
	std::vector<Buddy*> buddies;

	void initSize(size_t size);
	void* allocateBuddy(size_t buddyIndex, size_t size);
	size_t findBuddyIndex(void* ptr);
	void freeAndMerge(size_t i);
	size_t findNeighbor(size_t i);
public:
	BuddyAllocator(size_t size);
	void* alloc(size_t size);
	void dealloc(void* ptr);
	void show();
};

