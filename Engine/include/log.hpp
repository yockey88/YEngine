#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <stdlib.h>

#define MACHY_DEFAULT_LOGGER_NAME "Machy Log"
#define MACHY_ERROR_LOGGER_NAME "Machy Error Log"

#if defined(MACHY_PLATFORM_WINDOWS)
#define MACHY_BREAK __debugbreak();
#elif defined(MACHY_PLATFORM_MAC)
#define MACHY_BREAK __builtin_debugtrap();
#else
#define MACHY_BREAK __builtin_trap();
#endif

#define MACHY_INFO(...) \
    if (spdlog::get(MACHY_DEFAULT_LOGGER_NAME) != nullptr) { \
        spdlog::get(MACHY_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__);\
    }

#ifndef MACHY_CONFIG_RELEASE
#define MACHY_TRACE(...) \
    if (spdlog::get(MACHY_DEFAULT_LOGGER_NAME) != nullptr) { \
        spdlog::get(MACHY_DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__);\
    }
#define MACHY_DEBUG(...) \
    if (spdlog::get(MACHY_DEFAULT_LOGGER_NAME) != nullptr) { \
        spdlog::get(MACHY_DEFAULT_LOGGER_NAME)->debug(__VA_ARGS__);\
    }
#define MACHY_WARN(...) \
    if (spdlog::get(MACHY_ERROR_LOGGER_NAME) != nullptr) { \
        spdlog::get(MACHY_ERROR_LOGGER_NAME)->warn(__VA_ARGS__);\
    }
#define MACHY_ERROR(...) \
    if (spdlog::get(MACHY_ERROR_LOGGER_NAME) != nullptr) { \
        spdlog::get(MACHY_ERROR_LOGGER_NAME)->error(__VA_ARGS__);\
    }
#define MACHY_FATAL(...) \
    if (spdlog::get(MACHY_ERROR_LOGGER_NAME) != nullptr) { \
        spdlog::get(MACHY_ERROR_LOGGER_NAME)->critical(__VA_ARGS__);\
    }
#define MACHY_ASSERT(x , msg) \
    if ((x)) {} \
    else { MACHY_FATAL("|!|ASSERT|!| -> {}|\n\t\t|{}|\n\t\t| in file: {}|\n\t\t| in line: {}|" , x , msg , __FILE__ , __LINE__); MACHY_BREAK }
#else
#define MACHY_TRACE(...) (void)0
#define MACHY_TRACE(...) (void)0
#define MACHY_DEBUG(...) (void)0
#define MACHY_WARN(...) (void)0
#define MACHY_ERROR(...) (void)0
#define MACHY_FATAL(...) (void)0
#define MACHY_ASSERT(x , msg) (void*)0
#endif

#endif
