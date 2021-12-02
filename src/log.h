#pragma once
#include <memory>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

class Log
{
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetLogger() { return m_Logger; }

    private:
        static std::shared_ptr<spdlog::logger> m_Logger; 
};

#define TRACE(...)    Log::GetLogger()->trace(__VA_ARGS__)
#define INFO(...)     Log::GetLogger()->info(__VA_ARGS__)
#define WARN(...)     Log::GetLogger()->warn(__VA_ARGS__)
#define ERROR(...)    Log::GetLogger()->error(__VA_ARGS__)
#define CRITICAL(...) Log::GetLogger()->critical(__VA_ARGS__)