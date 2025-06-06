#include "znpch.hpp"
#include "Memory.hpp"

#include "Log.hpp"
#include "Zenith/Debug/Profiler.hpp"

#include <mutex>

namespace Zenith {

	static Zenith::AllocationStats s_GlobalStats;

	static bool s_InInit = false;

	void Allocator::Init()
	{
		if (s_Data)
			return;

		s_InInit = true;
		AllocatorData* data = (AllocatorData*)Allocator::AllocateRaw(sizeof(AllocatorData));
		new(data) AllocatorData();
		s_Data = data;
		s_InInit = false;
	}

	void* Allocator::AllocateRaw(size_t size)
	{
		return malloc(size);
	}

	void* Allocator::Allocate(size_t size)
	{
		if (s_InInit)
			return AllocateRaw(size);

		if (!s_Data)
			Init();

		void* memory = malloc(size);

		{
			std::scoped_lock<std::mutex> lock(s_Data->m_Mutex);
			Allocation& alloc = s_Data->m_AllocationMap[memory];
			alloc.Memory = memory;
			alloc.Size = size;

			s_GlobalStats.TotalAllocated += size;
		}

#if ZN_ENABLE_PROFILING
		TracyAlloc(memory, size);
#endif

		return memory;
	}

	void* Allocator::Allocate(size_t size, const char* desc)
	{
		if (!s_Data)
			Init();

		void* memory = malloc(size);

		{
			std::scoped_lock<std::mutex> lock(s_Data->m_Mutex);
			Allocation& alloc = s_Data->m_AllocationMap[memory];
			alloc.Memory = memory;
			alloc.Size = size;
			alloc.Category = desc;

			s_GlobalStats.TotalAllocated += size;
			if (desc)
				s_Data->m_AllocationStatsMap[desc].TotalAllocated += size;
		}

#if ZN_ENABLE_PROFILING
		TracyAlloc(memory, size);
#endif

		return memory;
	}

	void* Allocator::Allocate(size_t size, const char* file, int line)
	{
		if (!s_Data)
			Init();

		void* memory = malloc(size);

		{
			std::scoped_lock<std::mutex> lock(s_Data->m_Mutex);
			Allocation& alloc = s_Data->m_AllocationMap[memory];
			alloc.Memory = memory;
			alloc.Size = size;
			alloc.Category = file;

			s_GlobalStats.TotalAllocated += size;
			s_Data->m_AllocationStatsMap[file].TotalAllocated += size;
		}

#if ZN_ENABLE_PROFILING
		TracyAlloc(memory, size);
#endif

		return memory;
	}

	void Allocator::Free(void* memory)
	{
		if (memory == nullptr)
			return;

		{
			bool found = false;
			{
				std::scoped_lock<std::mutex> lock(s_Data->m_Mutex);
				auto allocMapIt = s_Data->m_AllocationMap.find(memory);
				found = allocMapIt != s_Data->m_AllocationMap.end();
				if (found)
				{
					const Allocation& alloc = allocMapIt->second;
					s_GlobalStats.TotalFreed += alloc.Size;
					if (alloc.Category)
						s_Data->m_AllocationStatsMap[alloc.Category].TotalFreed += alloc.Size;

					s_Data->m_AllocationMap.erase(memory);
				}
			}

#if ZN_ENABLE_PROFILING
			TracyFree(memory);
#endif

#ifndef ZN_DIST
			if (!found)
				ZN_CORE_WARN_TAG("Memory", "Memory block {0} not present in alloc map", memory);
#endif
		}

		free(memory);
	}

	namespace Memory {

		const AllocationStats& GetAllocationStats() { return s_GlobalStats; }
	}
}

#if defined(ZN_TRACK_MEMORY) && defined(ZN_PLATFORM_WINDOWS)

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t size)
{
	return Zenith::Allocator::Allocate(size);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t size)
{
	return Zenith::Allocator::Allocate(size);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t size, const char* desc)
{
	return Zenith::Allocator::Allocate(size, desc);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t size, const char* desc)
{
	return Zenith::Allocator::Allocate(size, desc);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t size, const char* file, int line)
{
	return Zenith::Allocator::Allocate(size, file, line);
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t size, const char* file, int line)
{
	return Zenith::Allocator::Allocate(size, file, line);
}

void __CRTDECL operator delete(void* memory)
{
	return Zenith::Allocator::Free(memory);
}

void __CRTDECL operator delete(void* memory, const char* desc)
{
	return Zenith::Allocator::Free(memory);
}

void __CRTDECL operator delete(void* memory, const char* file, int line)
{
	return Zenith::Allocator::Free(memory);
}

void __CRTDECL operator delete[](void* memory)
{
	return Zenith::Allocator::Free(memory);
}

void __CRTDECL operator delete[](void* memory, const char* desc)
{
	return Zenith::Allocator::Free(memory);
}

void __CRTDECL operator delete[](void* memory, const char* file, int line)
{
	return Zenith::Allocator::Free(memory);
}

#endif
