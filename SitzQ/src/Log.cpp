#include "Log.h"

using namespace SitzCue;

#ifdef DEBUG
Log::LogLevel Log::s_LogLevel = Log::LogLevel::LogLevelInfo;
#else
Log::LogLevel Log::s_LogLevel = Log::LogLevel::LogLevelError;
#endif