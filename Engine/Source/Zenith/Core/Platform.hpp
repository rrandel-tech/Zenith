#pragma once

#include <string>

namespace Zenith {

	class Platform
	{
	public:
		static uint64_t GetCurrentDateTimeU64();
		static std::string GetCurrentDateTimeString();
	};

}