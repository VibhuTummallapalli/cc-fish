#pragma once
#include "java_object/java_object.h"

class c_uuid : public c_java_object {
public:
    c_uuid ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    // Get the most significant bits
    [[nodiscard]] long long get_most_significant_bits ( ) const {
        static jmethodID method = nullptr;
        if ( !method ) {
            const jclass uuid_class = m_java->get_env (  )->FindClass ( xorstr_ ( "java/util/UUID" ) );
            method = m_java->get_env (  )->GetMethodID ( uuid_class, xorstr_ ( "getMostSignificantBits" ), xorstr_ ( "()J" ) );
            m_java->get_env (  )->DeleteLocalRef ( uuid_class );
        }
        return method ? m_java->get_env (  )->CallLongMethod ( m_obj, method ) : 0L;
    }

    // Get the least significant bits
    [[nodiscard]] long long get_least_significant_bits ( ) const {
        static jmethodID method = nullptr;
        if ( !method ) {
            const jclass uuid_class = m_java->get_env (  )->FindClass ( xorstr_ ( "java/util/UUID" ) );
            method = m_java->get_env (  )->GetMethodID ( uuid_class, xorstr_ ( "getLeastSignificantBits" ), xorstr_ ( "()J" ) );
            m_java->get_env (  )->DeleteLocalRef ( uuid_class );
        }
        return method ? m_java->get_env (  )->CallLongMethod ( m_obj, method ) : 0L;
    }

    // Get UUID as string (e.g., "550e8400-e29b-41d4-a716-446655440000")
    [[nodiscard]] std::string to_string ( ) const {
        static jmethodID method = nullptr;
        if ( !method ) {
            const jclass uuid_class = m_java->get_env (  )->FindClass ( xorstr_ ( "java/util/UUID" ) );
            method = m_java->get_env (  )->GetMethodID ( uuid_class, xorstr_ ( "toString" ), xorstr_ ( "()Ljava/lang/String;" ) );
            m_java->get_env (  )->DeleteLocalRef ( uuid_class );
        }

        if ( !method ) return "";

        jstring jstr = static_cast < jstring > ( m_java->get_env ( )->CallObjectMethod ( m_obj, method ) );
        if ( !jstr ) return "";

        const char * str = m_java->get_env (  )->GetStringUTFChars ( jstr, nullptr );
        std::string result ( str );
        m_java->get_env (  )->ReleaseStringUTFChars ( jstr, str );
        m_java->get_env (  )->DeleteLocalRef ( jstr );
        return result;
    }

	// Get as 128-bit value (most significant first, then least significant)
    struct uuid_value {
        long long most_significant;
        long long least_significant;

        bool operator==( const uuid_value & other ) const {
            return most_significant == other.most_significant &&
                least_significant == other.least_significant;
        }

        bool operator!=( const uuid_value & other ) const {
            return most_significant != other.most_significant ||
                least_significant != other.least_significant;
        }

        struct hash {
            std::size_t operator()( const uuid_value & uuid ) const noexcept {
                const std::size_t h1 = std::hash<long long> {}( uuid.most_significant );
                const std::size_t h2 = std::hash<long long> {}( uuid.least_significant );
                return h1 ^ ( h2 << 1 );
            }
        };
    };

    [[nodiscard]] uuid_value get_value ( ) const {
        return { .most_significant = get_most_significant_bits ( ), .least_significant = get_least_significant_bits ( ) };
    }

    // Comparison operators
    [[nodiscard]] bool equals ( const c_uuid & other ) const {
        if ( !is_valid ( ) || !other.is_valid ( ) ) return false;
        return get_value ( ) == other.get_value ( );
    }
};
