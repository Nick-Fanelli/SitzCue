#pragma once

#include "sitzqpch.h"

namespace SitzQ {

    namespace Log {

        static const char* ANSI_RESET = "\033[0m";
    
        static const char* ANSI_RED = "\033[0;31m";
        static const char* ANSI_RED_BOLD = "\033[1;31m";

        static const char* ANSI_YELLOW = "\033[0;33m";
        static const char* ANSI_YELLOW_BOLD = "\033[1;33m";

        static const char* ANSI_BLUE_BOLD = "\033[38;5;153;1m";
        static const char* ANSI_BLUE = "\033[0;38;5;153m";

        static const char* ANSI_GREEN = "\033[0;32m";
        static const char* ANSI_GREEN_BOLD = "\033[1;32m";

        enum LogLevel {
            LogLevelError = 0, LogLevelWarn = 1, LogLevelInfo = 2, LogLevelTrace = 3, LogLevelDebug = 4
        };

#ifdef SITZQ_DEBUG
        static LogLevel s_LogLevel = LogLevel::LogLevelDebug;
#else
        static LogLevel s_LogLevel = LogLevel::LogLevelError;
#endif

        static bool s_ShouldDisplayThreadInformation = false;
        static bool s_ShouldDisplayTimestampInformation = true;

        inline bool ShouldDisplayThreadInformation() { return s_ShouldDisplayThreadInformation; }
        inline void SetShouldDisplayThreadInformation(bool shouldDisplay) { s_ShouldDisplayThreadInformation = shouldDisplay; }

        inline bool ShouldDisplayTimestampInformation() { return s_ShouldDisplayTimestampInformation; }
        inline void SetShouldDisplayTimestampInformation(bool shouldDisplay) { s_ShouldDisplayTimestampInformation = shouldDisplay; }

        inline LogLevel GetLogLevel() { return s_LogLevel; }
        inline void SetLogLevel(LogLevel logLevel) { s_LogLevel = logLevel; }

        static void FormatString(std::ostringstream& oss, const std::string& format) {
            oss << format;
        }

        template<typename T, typename... Args>
        static void FormatString(std::ostringstream& oss, const std::string& format, const T& arg, Args... args) {

            size_t pos = format.find("{}");

            if(pos != std::string::npos) {
                oss << format.substr(0, pos) << arg;
                FormatString(oss, format.substr(pos + 2), args...);
            } else {
                oss << format;
            }
        }

        static inline void GetTimeStamp(std::ostringstream& oss) {

            if(!s_ShouldDisplayTimestampInformation)  
                return;

            auto now = std::chrono::system_clock::now();
            std::time_t time = std::chrono::system_clock::to_time_t(now);

            oss << "[";

            std::tm* localTime = std::localtime(&time);
            if(localTime != nullptr) {
                oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

                // Obtain millisecond
                auto duration = now.time_since_epoch();
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
                oss << "." << std::setfill('0') << std::setw(3) << milliseconds;
            }

            oss << "] ";

        }

        static inline void GetThread(std::ostringstream& oss) {
            if(s_ShouldDisplayThreadInformation)
                oss << "[thread " << std::this_thread::get_id() << "] ";
        }

        template<typename T>
        void Trace(const T& data) {

            if(s_LogLevel < LogLevelTrace)
                return;

            std::ostringstream oss;

            GetTimeStamp(oss);
            GetThread(oss);

            oss << "[trace] " << data;

            std::cout << oss.str() << "\n";
        }

        template<typename... Args>
        void Trace(const std::string& format, Args... args) {

            if(s_LogLevel < LogLevelTrace)
                return;

            std::ostringstream oss;

            GetTimeStamp(oss);
            GetThread(oss);

            oss << "[trace] ";

            FormatString(oss, format, args...);

            std::cout << oss.str() << "\n";
        }

        template<typename T>
        void Info(const T& data) {
            if(s_LogLevel < LogLevelInfo)
                return;

            std::ostringstream oss;

            oss << ANSI_BLUE_BOLD;

            GetTimeStamp(oss);
            GetThread(oss);

            oss << "[info] " << ANSI_BLUE << data;

            std::cout << oss.str() << ANSI_RESET << "\n";
        } 

        template<typename... Args>
        void Info(const std::string& format, Args... args) {

            if(s_LogLevel < LogLevelInfo)
                return;

            std::ostringstream oss;

            oss << ANSI_BLUE_BOLD;

            GetTimeStamp(oss);
            GetThread(oss);

            oss << "[info] " << ANSI_BLUE;

            FormatString(oss, format, args...);

            std::cout << oss.str() << ANSI_RESET << "\n";
        }

        template<typename T>
        void Warn(const T& data) {

            if(s_LogLevel < LogLevelWarn)
                return;

            std::ostringstream oss;

            oss << ANSI_YELLOW_BOLD;

            GetTimeStamp(oss);
            GetThread(oss);

            oss << "[WARN] " << ANSI_YELLOW << data;

            std::cout << oss.str() << ANSI_RESET << "\n";
        }

        template<typename... Args>
        void Warn(const std::string& format, Args... args) {

            if(s_LogLevel < LogLevelWarn)
                return;

            std::ostringstream oss;

            oss << ANSI_YELLOW_BOLD;

            GetTimeStamp(oss);
            GetThread(oss);

            oss << "[WARN] " << ANSI_YELLOW;

            FormatString(oss, format, args...);

            std::cout << oss.str() << ANSI_RESET << "\n";
        }

        template<typename T>
        void Error(const T& data) {

            if(s_LogLevel < LogLevelError)
                return;

            std::ostringstream oss;

            oss << ANSI_RED_BOLD;

            GetTimeStamp(oss);
            GetThread(oss);

            oss << "[ERROR] " << ANSI_RED << data;

            std::cout << oss.str() << ANSI_RESET << "\n";
        }

        template<typename... Args>
        void Error(const std::string& format, Args... args) {

            if(s_LogLevel < LogLevelError)
                return;

            std::ostringstream oss;

            oss << ANSI_RED_BOLD;

            GetTimeStamp(oss);
            GetThread(oss);

            oss << "[ERROR] " << ANSI_RED;

            FormatString(oss, format, args...);

            std::cout << oss.str() << ANSI_RESET << "\n";
        }

        template<typename T>
        void Debug(const T& data) {

            if(s_LogLevel < LogLevelDebug)
                return;

            std::ostringstream oss;

            oss << ANSI_GREEN_BOLD;

            GetTimeStamp(oss);
            GetThread(oss);

            oss << "[DEBUG] " << ANSI_GREEN << data;

            std::cout << oss.str() << ANSI_RESET << "\n";
        }

        template<typename... Args>
        void Debug(const std::string& format, Args... args) {

            if(s_LogLevel < LogLevelDebug)
                return;

            std::ostringstream oss;

            oss << ANSI_GREEN_BOLD;

            GetTimeStamp(oss);
            GetThread(oss);

            oss << "[DEBUG] " << ANSI_GREEN;

            FormatString(oss, format, args...);

            std::cout << oss.str() << ANSI_RESET << "\n";
        }

    }


}