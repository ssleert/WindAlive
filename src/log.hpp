#ifndef LOG_HPP
#define LOG_HPP

#ifdef NDEBUG
  #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_WARN
#else
  #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif

#include <spdlog/spdlog.h>

#endif
