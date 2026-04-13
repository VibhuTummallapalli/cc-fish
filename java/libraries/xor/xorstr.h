#pragma once
/*
 * XOR String Encryption Library
 * 
 * This is a simple compile-time string encryption utility.
 * For development/debugging, encryption is disabled.
 * 
 * To enable actual encryption, replace this with a proper xorstr library like:
 * https://github.com/JustasMasiulis/xorstr
 */

#include <string>
#include <cstring>

// Development mode - no encryption (easier to debug)
// Comment out this line and use the encrypted version below for release builds
#define XORSTR_DEVELOPMENT_MODE

#ifdef XORSTR_DEVELOPMENT_MODE

// Simple pass-through macro for development
#define xorstr_(str) str
#define xorstr(str) str

#else

// Compile-time XOR encryption
namespace xor_detail {
    constexpr auto key = __TIME__[0] ^ __TIME__[1] ^ __TIME__[3] ^ __TIME__[4] ^ __TIME__[6] ^ __TIME__[7];

    template<std::size_t N>
    struct encrypted_string {
        char data[N];

        constexpr encrypted_string(const char(&str)[N]) {
            for (std::size_t i = 0; i < N; ++i) {
                data[i] = str[i] ^ (key + i);
            }
        }

        std::string decrypt() const {
            std::string result;
            result.reserve(N - 1);
            for (std::size_t i = 0; i < N - 1; ++i) {
                result.push_back(data[i] ^ (key + i));
            }
            return result;
        }

        const char* c_str() const {
            static thread_local char decrypted[N];
            for (std::size_t i = 0; i < N; ++i) {
                decrypted[i] = data[i] ^ (key + i);
            }
            return decrypted;
        }
    };

    template<std::size_t N>
    constexpr auto make_encrypted(const char(&str)[N]) {
        return encrypted_string<N>(str);
    }
}

#define xorstr_(str) (::xor_detail::make_encrypted(str).c_str())
#define xorstr(str) xorstr_(str)

#endif // XORSTR_DEVELOPMENT_MODE

// String literal operator for convenience
// Usage: "Hello World"_xor
inline namespace xor_literals {
    inline const char* operator""_xor(const char* str, std::size_t) {
        return str; // In development mode, just return as-is
    }
}
