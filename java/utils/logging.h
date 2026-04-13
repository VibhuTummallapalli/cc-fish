#pragma once
#include <iostream>
#include <string>
#include <sstream>

// Simple terminal formatting namespace (inspired by oof library)
namespace oof {
    struct color {
        int r, g, b;
    };

    inline std::string fg_color ( color c ) {
        std::ostringstream oss;
        oss << "\033[38;2;" << c.r << ";" << c.g << ";" << c.b << "m";
        return oss.str ( );
    }

    inline std::string bg_color ( color c ) {
        std::ostringstream oss;
        oss << "\033[48;2;" << c.r << ";" << c.g << ";" << c.b << "m";
        return oss.str ( );
    }

    inline std::string reset_formatting ( ) {
        return "\033[0m";
    }

    inline std::string bold ( ) {
        return "\033[1m";
    }
}

// Color definitions
namespace log_colors {
    constexpr oof::color success = { 100, 255, 100 };
    constexpr oof::color info = { 150, 200, 255 };
    constexpr oof::color warning = { 255, 200, 100 };
    constexpr oof::color error = { 255, 100, 100 };
    constexpr oof::color prefix = { 200, 150, 255 };
}

// Logging macros
#define LOG_PREFIX "[Fish] "

#define LOG_SUCCESS(msg) \
    std::cout << oof::fg_color(log_colors::prefix) << LOG_PREFIX \
              << oof::fg_color(log_colors::success) << "[+] " \
              << oof::reset_formatting() << msg \
              << oof::reset_formatting() << std::endl

#define LOG_INFO(msg) \
    std::cout << oof::fg_color(log_colors::prefix) << LOG_PREFIX \
              << oof::fg_color(log_colors::info) << "[*] " \
              << oof::reset_formatting() << msg \
              << oof::reset_formatting() << std::endl

#define LOG_WARNING(msg) \
    std::cout << oof::fg_color(log_colors::prefix) << LOG_PREFIX \
              << oof::fg_color(log_colors::warning) << "[!] " \
              << oof::reset_formatting() << msg \
              << oof::reset_formatting() << std::endl

#define LOG_ERROR(msg) \
    std::cout << oof::fg_color(log_colors::prefix) << LOG_PREFIX \
              << oof::fg_color(log_colors::error) << "[-] " \
              << oof::reset_formatting() << msg \
              << oof::reset_formatting() << std::endl

#define LOG_DEBUG(msg) \
    std::cout << oof::fg_color(log_colors::prefix) << LOG_PREFIX \
              << oof::fg_color({150, 150, 150}) << "[D] " \
              << oof::reset_formatting() << msg \
              << oof::reset_formatting() << std::endl
