#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

// Global log file - defined in dllmain_file.cpp
extern std::ofstream g_log_file;

// File-based logging that works without a console
inline void fish_log_impl ( const std::string & level, const std::string & msg ) {
    if ( g_log_file.is_open ( ) ) {
        auto now = std::chrono::system_clock::now ( );
        auto time = std::chrono::system_clock::to_time_t ( now );
        std::tm tm_buf;
        localtime_s ( &tm_buf, &time );
        
        g_log_file << "[" << std::put_time ( &tm_buf, "%H:%M:%S" ) << "] "
                   << "[" << level << "] " << msg << std::endl;
        g_log_file.flush ( );
    }
}

#define LOG_INFO(msg) { std::ostringstream _ss; _ss << msg; fish_log_impl("INFO", _ss.str()); }
#define LOG_SUCCESS(msg) { std::ostringstream _ss; _ss << msg; fish_log_impl("OK", _ss.str()); }
#define LOG_WARNING(msg) { std::ostringstream _ss; _ss << msg; fish_log_impl("WARN", _ss.str()); }
#define LOG_ERROR(msg) { std::ostringstream _ss; _ss << msg; fish_log_impl("ERROR", _ss.str()); }

// Compatibility with old oof:: namespace (just return empty strings since we're not using colors)
namespace oof {
    struct color { int r, g, b; };
    inline std::string fg_color ( color ) { return ""; }
    inline std::string bg_color ( color ) { return ""; }
    inline std::string reset_formatting ( ) { return ""; }
    inline std::string bold ( ) { return ""; }
}
