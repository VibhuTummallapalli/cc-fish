#pragma once
#include <cmath>
#include <algorithm>

template<typename T>
struct vec2 {
    T x, y;

    vec2 ( ) : x ( 0 ), y ( 0 ) { }
    vec2 ( T x, T y ) : x ( x ), y ( y ) { }

    vec2 operator+ ( const vec2 & other ) const { return { x + other.x, y + other.y }; }
    vec2 operator- ( const vec2 & other ) const { return { x - other.x, y - other.y }; }
    vec2 operator* ( T scalar ) const { return { x * scalar, y * scalar }; }
    vec2 operator/ ( T scalar ) const { return { x / scalar, y / scalar }; }

    vec2 & operator+= ( const vec2 & other ) { x += other.x; y += other.y; return *this; }
    vec2 & operator-= ( const vec2 & other ) { x -= other.x; y -= other.y; return *this; }
    vec2 & operator*= ( T scalar ) { x *= scalar; y *= scalar; return *this; }
    vec2 & operator/= ( T scalar ) { x /= scalar; y /= scalar; return *this; }

    bool operator== ( const vec2 & other ) const { return x == other.x && y == other.y; }
    bool operator!= ( const vec2 & other ) const { return !( *this == other ); }

    [[nodiscard]] T length ( ) const { return std::sqrt ( x * x + y * y ); }
    [[nodiscard]] T length_sqr ( ) const { return x * x + y * y; }
    [[nodiscard]] T dot ( const vec2 & other ) const { return x * other.x + y * other.y; }
    
    [[nodiscard]] vec2 normalized ( ) const {
        T len = length ( );
        return len > 0 ? vec2 ( x / len, y / len ) : vec2 ( );
    }

    [[nodiscard]] T distance ( const vec2 & other ) const {
        return ( *this - other ).length ( );
    }
};

template<typename T>
struct vec3 {
    T x, y, z;

    vec3 ( ) : x ( 0 ), y ( 0 ), z ( 0 ) { }
    vec3 ( T x, T y, T z ) : x ( x ), y ( y ), z ( z ) { }

    vec3 operator+ ( const vec3 & other ) const { return { x + other.x, y + other.y, z + other.z }; }
    vec3 operator- ( const vec3 & other ) const { return { x - other.x, y - other.y, z - other.z }; }
    vec3 operator* ( T scalar ) const { return { x * scalar, y * scalar, z * scalar }; }
    vec3 operator/ ( T scalar ) const { return { x / scalar, y / scalar, z / scalar }; }
    vec3 operator- ( ) const { return { -x, -y, -z }; }

    vec3 & operator+= ( const vec3 & other ) { x += other.x; y += other.y; z += other.z; return *this; }
    vec3 & operator-= ( const vec3 & other ) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    vec3 & operator*= ( T scalar ) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    vec3 & operator/= ( T scalar ) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    bool operator== ( const vec3 & other ) const { return x == other.x && y == other.y && z == other.z; }
    bool operator!= ( const vec3 & other ) const { return !( *this == other ); }

    [[nodiscard]] T length ( ) const { return std::sqrt ( x * x + y * y + z * z ); }
    [[nodiscard]] T length_sqr ( ) const { return x * x + y * y + z * z; }
    [[nodiscard]] T length_2d ( ) const { return std::sqrt ( x * x + z * z ); }

    [[nodiscard]] T dot ( const vec3 & other ) const { 
        return x * other.x + y * other.y + z * other.z; 
    }

    [[nodiscard]] vec3 cross ( const vec3 & other ) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    [[nodiscard]] vec3 normalized ( ) const {
        T len = length ( );
        return len > 0 ? vec3 ( x / len, y / len, z / len ) : vec3 ( );
    }

    [[nodiscard]] T distance ( const vec3 & other ) const {
        return ( *this - other ).length ( );
    }

    [[nodiscard]] T distance_2d ( const vec3 & other ) const {
        T dx = x - other.x;
        T dz = z - other.z;
        return std::sqrt ( dx * dx + dz * dz );
    }

    // Linear interpolation
    [[nodiscard]] vec3 lerp ( const vec3 & other, T t ) const {
        return *this + ( other - *this ) * t;
    }

    // Clamp each component
    [[nodiscard]] vec3 clamped ( T min_val, T max_val ) const {
        return {
            std::clamp ( x, min_val, max_val ),
            std::clamp ( y, min_val, max_val ),
            std::clamp ( z, min_val, max_val )
        };
    }

    // Convert to angles (pitch, yaw)
    [[nodiscard]] vec2<T> to_angles ( ) const {
        T yaw = std::atan2 ( z, x ) * static_cast<T> ( 180.0 / 3.14159265358979323846 );
        T pitch = std::atan2 ( -y, length_2d ( ) ) * static_cast<T> ( 180.0 / 3.14159265358979323846 );
        return { pitch, yaw };
    }
};

template<typename T>
struct vec4 {
    T x, y, z, w;

    vec4 ( ) : x ( 0 ), y ( 0 ), z ( 0 ), w ( 0 ) { }
    vec4 ( T x, T y, T z, T w ) : x ( x ), y ( y ), z ( z ), w ( w ) { }
    vec4 ( const vec3<T> & v, T w ) : x ( v.x ), y ( v.y ), z ( v.z ), w ( w ) { }

    vec4 operator+ ( const vec4 & other ) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
    vec4 operator- ( const vec4 & other ) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
    vec4 operator* ( T scalar ) const { return { x * scalar, y * scalar, z * scalar, w * scalar }; }
    vec4 operator/ ( T scalar ) const { return { x / scalar, y / scalar, z / scalar, w / scalar }; }

    [[nodiscard]] vec3<T> xyz ( ) const { return { x, y, z }; }
};

// Common type aliases
using vec2f = vec2<float>;
using vec2d = vec2<double>;
using vec2i = vec2<int>;

using vec3f = vec3<float>;
using vec3d = vec3<double>;
using vec3i = vec3<int>;

using vec4f = vec4<float>;
using vec4d = vec4<double>;
