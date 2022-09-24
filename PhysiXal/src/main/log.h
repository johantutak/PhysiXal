#pragma once

#include "main/core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace PhysiXal {

	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define PX_CORE_TRACE(...)    ::PhysiXal::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PX_CORE_INFO(...)     ::PhysiXal::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PX_CORE_WARN(...)     ::PhysiXal::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PX_CORE_ERROR(...)    ::PhysiXal::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PX_CORE_CRITICAL(...) ::PhysiXal::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define PX_TRACE(...)         ::PhysiXal::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PX_INFO(...)          ::PhysiXal::Log::GetClientLogger()->info(__VA_ARGS__)
#define PX_WARN(...)          ::PhysiXal::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PX_ERROR(...)         ::PhysiXal::Log::GetClientLogger()->error(__VA_ARGS__)
#define PX_CRITICAL(...)      ::PhysiXal::Log::GetClientLogger()->critical(__VA_ARGS__)