#pragma once
#include <array>
#include <cmath>
#include <functional>
#include "vec.h"

template<typename T>
struct matrix4 {
    std::array<T, 16> m;  // Column-major order (OpenGL style)

    matrix4 ( ) : m { } { }

    matrix4 ( std::array<T, 16> values ) : m ( values ) { }

    // Identity matrix
    static matrix4 identity ( ) {
        matrix4 result;
        result.m = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        return result;
    }

    // Create matrix from a getter function (for JNI FloatBuffer access)
    template<typename Func>
    static matrix4 from_getter ( Func getter ) {
        matrix4 result;
        for ( int i = 0; i < 16; ++i ) {
            result.m [ i ] = static_cast<T> ( getter ( i ) );
        }
        return result;
    }

    // Access elements (column-major: m[col * 4 + row])
    T & operator() ( int row, int col ) { return m [ col * 4 + row ]; }
    const T & operator() ( int row, int col ) const { return m [ col * 4 + row ]; }

    // Array-style access
    T & operator[] ( int index ) { return m [ index ]; }
    const T & operator[] ( int index ) const { return m [ index ]; }

    // Matrix multiplication
    matrix4 operator* ( const matrix4 & other ) const {
        matrix4 result;
        for ( int col = 0; col < 4; ++col ) {
            for ( int row = 0; row < 4; ++row ) {
                T sum = 0;
                for ( int k = 0; k < 4; ++k ) {
                    sum += ( *this ) ( row, k ) * other ( k, col );
                }
                result ( row, col ) = sum;
            }
        }
        return result;
    }

    // Transform a vec3 (assumes w = 1)
    vec3<T> transform ( const vec3<T> & v ) const {
        T x = m [ 0 ] * v.x + m [ 4 ] * v.y + m [ 8 ] * v.z + m [ 12 ];
        T y = m [ 1 ] * v.x + m [ 5 ] * v.y + m [ 9 ] * v.z + m [ 13 ];
        T z = m [ 2 ] * v.x + m [ 6 ] * v.y + m [ 10 ] * v.z + m [ 14 ];
        T w = m [ 3 ] * v.x + m [ 7 ] * v.y + m [ 11 ] * v.z + m [ 15 ];
        
        if ( std::abs ( w ) > 0.0001 ) {
            return { x / w, y / w, z / w };
        }
        return { x, y, z };
    }

    // Transform a vec4
    vec4<T> transform ( const vec4<T> & v ) const {
        return {
            m [ 0 ] * v.x + m [ 4 ] * v.y + m [ 8 ] * v.z + m [ 12 ] * v.w,
            m [ 1 ] * v.x + m [ 5 ] * v.y + m [ 9 ] * v.z + m [ 13 ] * v.w,
            m [ 2 ] * v.x + m [ 6 ] * v.y + m [ 10 ] * v.z + m [ 14 ] * v.w,
            m [ 3 ] * v.x + m [ 7 ] * v.y + m [ 11 ] * v.z + m [ 15 ] * v.w
        };
    }

    // Transpose
    [[nodiscard]] matrix4 transposed ( ) const {
        matrix4 result;
        for ( int col = 0; col < 4; ++col ) {
            for ( int row = 0; row < 4; ++row ) {
                result ( row, col ) = ( *this ) ( col, row );
            }
        }
        return result;
    }

    // Get raw data pointer
    T * data ( ) { return m.data ( ); }
    const T * data ( ) const { return m.data ( ); }

    // Translation matrix
    static matrix4 translate ( T x, T y, T z ) {
        matrix4 result = identity ( );
        result.m [ 12 ] = x;
        result.m [ 13 ] = y;
        result.m [ 14 ] = z;
        return result;
    }

    static matrix4 translate ( const vec3<T> & v ) {
        return translate ( v.x, v.y, v.z );
    }

    // Scale matrix
    static matrix4 scale ( T x, T y, T z ) {
        matrix4 result = identity ( );
        result.m [ 0 ] = x;
        result.m [ 5 ] = y;
        result.m [ 10 ] = z;
        return result;
    }

    static matrix4 scale ( const vec3<T> & v ) {
        return scale ( v.x, v.y, v.z );
    }

    // Rotation around X axis
    static matrix4 rotate_x ( T angle_radians ) {
        T c = std::cos ( angle_radians );
        T s = std::sin ( angle_radians );
        matrix4 result = identity ( );
        result.m [ 5 ] = c;
        result.m [ 6 ] = s;
        result.m [ 9 ] = -s;
        result.m [ 10 ] = c;
        return result;
    }

    // Rotation around Y axis
    static matrix4 rotate_y ( T angle_radians ) {
        T c = std::cos ( angle_radians );
        T s = std::sin ( angle_radians );
        matrix4 result = identity ( );
        result.m [ 0 ] = c;
        result.m [ 2 ] = -s;
        result.m [ 8 ] = s;
        result.m [ 10 ] = c;
        return result;
    }

    // Rotation around Z axis
    static matrix4 rotate_z ( T angle_radians ) {
        T c = std::cos ( angle_radians );
        T s = std::sin ( angle_radians );
        matrix4 result = identity ( );
        result.m [ 0 ] = c;
        result.m [ 1 ] = s;
        result.m [ 4 ] = -s;
        result.m [ 5 ] = c;
        return result;
    }

    // Perspective projection matrix
    static matrix4 perspective ( T fov_radians, T aspect, T near_plane, T far_plane ) {
        T f = static_cast<T> ( 1 ) / std::tan ( fov_radians / static_cast<T> ( 2 ) );
        T nf = static_cast<T> ( 1 ) / ( near_plane - far_plane );

        matrix4 result;
        result.m [ 0 ] = f / aspect;
        result.m [ 1 ] = 0;
        result.m [ 2 ] = 0;
        result.m [ 3 ] = 0;
        result.m [ 4 ] = 0;
        result.m [ 5 ] = f;
        result.m [ 6 ] = 0;
        result.m [ 7 ] = 0;
        result.m [ 8 ] = 0;
        result.m [ 9 ] = 0;
        result.m [ 10 ] = ( far_plane + near_plane ) * nf;
        result.m [ 11 ] = -1;
        result.m [ 12 ] = 0;
        result.m [ 13 ] = 0;
        result.m [ 14 ] = ( 2 * far_plane * near_plane ) * nf;
        result.m [ 15 ] = 0;
        return result;
    }

    // Orthographic projection matrix
    static matrix4 orthographic ( T left, T right, T bottom, T top, T near_plane, T far_plane ) {
        matrix4 result = identity ( );
        result.m [ 0 ] = static_cast<T> ( 2 ) / ( right - left );
        result.m [ 5 ] = static_cast<T> ( 2 ) / ( top - bottom );
        result.m [ 10 ] = static_cast<T> ( -2 ) / ( far_plane - near_plane );
        result.m [ 12 ] = -( right + left ) / ( right - left );
        result.m [ 13 ] = -( top + bottom ) / ( top - bottom );
        result.m [ 14 ] = -( far_plane + near_plane ) / ( far_plane - near_plane );
        return result;
    }
};

// Type aliases
using matrix4f = matrix4<float>;
using matrix4d = matrix4<double>;
