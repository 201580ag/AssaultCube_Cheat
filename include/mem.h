#pragma once
#include <vector>   // std::vector를 사용하기 위해 필요합니다.
#include <cstdint>  // uintptr_t 타입을 사용하기 위해 필요합니다.
#include <Windows.h>

namespace mem
{
	// Chain을 구하는코드
	uintptr_t FindAddress(uintptr_t ptr, std::vector<unsigned int> offsets)
	{
		uintptr_t address = ptr;
		for (unsigned int i = 0; i < offsets.size(); ++i)
		{
			address = *(uintptr_t*)address;
			address += offsets[i];
		}
		return address;
	};
}