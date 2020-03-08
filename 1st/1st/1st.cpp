#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <map>
#include <time.h>
#include <vector>
#include <cstdlib>
#include <string>
#include <iterator>
#include <algorithm>


struct cmp_str
{
	bool operator()(char const* a, char const* b) const
	{
		return std::strcmp(a, b) < 0;
	}
};

template <class T>
bool cmp(const std::pair<T, size_t>& a, const std::pair<T, size_t>& b)
{
	return a.second < b.second;
}

template <class T>
struct Mallocator
{
	typedef T value_type;
	std::vector<void*> buffers;
	size_t free = 0;
	T* ptr = NULL;

	Mallocator() = default;
	/*~Mallocator()
	{
		for (auto buf : buffers)
		{
			std::free(buf);
		}
	}*/
	template <class U> constexpr Mallocator(const Mallocator <U>&) noexcept {}

	T* allocate(std::size_t n) {
		if (free < n)
		{
			ptr = static_cast<T*>(std::malloc(1048577));
			buffers.push_back(ptr);
			free = 1048577;
		}
		free -= sizeof(T) * n;
		return ptr++;
	}
	void deallocate(T* p, std::size_t) noexcept {}
};

template <class T, class U>
bool operator==(const Mallocator <T>&, const Mallocator <U>&) { return true; }
template <class T, class U>
bool operator!=(const Mallocator <T>&, const Mallocator <U>&) { return false; }


char* GetFileText(LPCWSTR filename)
{
	OFSTRUCT* buff = (OFSTRUCT*)malloc(sizeof(OFSTRUCT));
	HANDLE hFile = CreateFile(
		filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return (char*)"not found";
	}
	size_t fileSize = GetFileSize(hFile, NULL);
	char* ReadBuffer = new char[fileSize + 1];
	OVERLAPPED ol = { 0 };
	LPDWORD readed = 0;
	if (TRUE == ReadFile(hFile, ReadBuffer, fileSize, readed, NULL))
	{
		return ReadBuffer;
	}
	return (char*)"smth wrong";
}

void AnalyzeCustomAlloc(char* textBuffer, uint32_t resCount)
{
	float fTimeStart = clock() / (float)CLOCKS_PER_SEC;
	char* token = strtok(textBuffer, " ,.!?:;\n\r\t\"'[]-_()");
	std::map<char*, size_t, cmp_str, Mallocator<std::pair<char*, size_t> >> counter;
	while (token != NULL)
	{
		counter[token]++;
		token = strtok(NULL, " ,.!?:;\n\r\t\"'[]-_()");
	}
	float fTimeStop = clock() / (float)CLOCKS_PER_SEC;
	std::vector<std::pair<char*, size_t>> result;
	for (auto smth : counter)
	{
		result.push_back(smth);
	}
	std::sort(result.begin(), result.end(), cmp<char*>);
	for (auto i = result.size() - 1; i > result.size() - resCount; i--)
	{
		std::cout << result[i].first << "  " << result[i].second << "\n";
	}
	std::cout << fTimeStop - fTimeStart << "sec" << "\n";
}

void AnalyzeStrAlloc(char* textBuffer, uint32_t resCount)
{
	float fTimeStart = clock() / (float)CLOCKS_PER_SEC;
	char* token = strtok(textBuffer, " ,.!?:;\n\r\t\"'[]-_()");
	std::map<std::string, size_t> counter;
	while (token != NULL)
	{
		counter[(std::string)token]++;
		token = strtok(NULL, " ,.!?:;\n\r\t\"'[]-_()");
	}
	float fTimeStop = clock() / (float)CLOCKS_PER_SEC;
	std::vector<std::pair<std::string, size_t>> result;
	for (auto smth : counter)
	{
		result.push_back(smth);
	}
	std::sort(result.begin(), result.end(), cmp<std::string>);
	for (auto i = result.size() - 1; i > result.size() - resCount; i--)
	{
		std::cout << result[i].first << "  " << result[i].second << "\n";
	}
	std::cout << fTimeStop - fTimeStart << "sec" << "\n";
}

int main()
{
	char* textBuffer = GetFileText((LPCWSTR)L"book.txt");
	//AnalyzeStrAlloc(textBuffer, 10);
	AnalyzeCustomAlloc(textBuffer, 10);
}