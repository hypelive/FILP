#include "BuddyAllocator.h"
#include <cmath>
#include <iostream>

void BuddyAllocator::initSize(size_t size)
{
	auto result = 0;
	size_t resultSize = 1;
	while (resultSize < size)
	{
		resultSize *= 2;
		result++;
	}

	maxSize = resultSize;
	levelsCount = result;
}

void* BuddyAllocator::allocateBuddy(size_t buddyIndex, size_t size)
{
	auto newBuddies = new std::vector<Buddy*>();
	newBuddies->push_back(buddies[buddyIndex]);
	while ((*newBuddies)[0]->size / 2 >= size)
	{
		newBuddies->erase(newBuddies->begin());
		if (newBuddies->size() > 0)
		{
			newBuddies->insert(newBuddies->begin(), new Buddy((*newBuddies)[0]->size / 2, nullptr));
			newBuddies->insert(newBuddies->begin(), new Buddy((*newBuddies)[0]->size, nullptr));
		}
		else
		{
			newBuddies->push_back(new Buddy(buddies[buddyIndex]->size / 2, nullptr));
			newBuddies->push_back(new Buddy(buddies[buddyIndex]->size / 2, nullptr));
		}
	}
	buddies.insert(buddies.begin() + buddyIndex, newBuddies->begin(), newBuddies->end());
	buddies.erase(buddies.begin() + buddyIndex + newBuddies->size());
	buddies[buddyIndex]->data = malloc(size);
	return buddies[buddyIndex]->data;
}

size_t BuddyAllocator::findBuddyIndex(void* ptr)
{
	for (size_t i = 0; i < buddies.size(); i++)
	{
		if (buddies[i]->data == ptr)
		{
			return i;
		}
	}

	return -1;
}

void BuddyAllocator::freeAndMerge(size_t i)
{
	if (buddies[i]->size == maxSize)
	{
		return;
	}

	auto currentBuddy = new Buddy(buddies[i]->size * 2, nullptr);
	size_t newIndex = 0;
	auto neighborIndex = findNeighbor(i);
	if (buddies[neighborIndex]->size == buddies[i]->size && buddies[neighborIndex]->isEmpty())
	{
		if (neighborIndex > i)
		{
			buddies.erase(buddies.begin() + i, buddies.begin() + neighborIndex + 1);
			newIndex = i;
		}
		else if (neighborIndex < i)
		{
			buddies.erase(buddies.begin() + neighborIndex, buddies.begin() + i + 1);
			newIndex = i - 1;
		}
	}
	else
	{
		return;
	}
	buddies.insert(buddies.begin() + newIndex, currentBuddy);
	freeAndMerge(newIndex);
}

size_t BuddyAllocator::findNeighbor(size_t i)
{
	size_t sum = 0;
	for (size_t j = 0; j < i; j++)
	{
		sum += buddies[j]->size;
	}
	return (sum / buddies[i]->size) % 2 == 0 ? i + 1 : i - 1;
}

BuddyAllocator::BuddyAllocator(size_t size)
{
	initSize(size);
	buddies.push_back(new Buddy(maxSize, nullptr));
}

void* BuddyAllocator::alloc(size_t size)
{
	for (size_t i = 0; i < buddies.size(); i++)
	{
		if (buddies[i]->isEmpty() && buddies[i]->size >= size)
		{
			return allocateBuddy(i, size);
		}
	}

	return nullptr;
}

void BuddyAllocator::dealloc(void* ptr)
{
	auto index = findBuddyIndex(ptr);
	if (index == (size_t)-1)
	{
		return;
	}
	free(buddies[index]->data);
	buddies[index]->data = nullptr;

	freeAndMerge(index);
}

void BuddyAllocator::show()
{
	//debug information
	std::cout << maxSize << "  " << levelsCount << "  " << buddies.size() << "\n";
}
