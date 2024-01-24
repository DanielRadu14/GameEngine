#pragma once

#include <memory>
#include "spdlog/sinks/stdout_color_sinks.h"

#include "Core.h"
#include "spdlog/spdlog.h"

namespace GameEngine {
	class GAMEENGINE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

