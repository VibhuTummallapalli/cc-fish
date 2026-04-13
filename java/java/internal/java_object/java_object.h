#pragma once
#include "../../java/java.h"

// Base wrapper class for Java objects with RAII
class c_java_object {
protected:
    jobject m_obj;
    c_java * m_java;
    bool m_owns_ref; // Whether we need to delete the local ref

    c_java_object ( const jobject obj, c_java * java, const bool owns_ref = true )
        : m_obj ( obj ), m_java ( java ), m_owns_ref ( owns_ref ) {
    }

public:
    virtual ~c_java_object ( ) {
        if ( m_owns_ref && m_obj && m_java ) {
        	m_java->get_env ( )->DeleteLocalRef ( m_obj );
        }
    }

    // Disable copy, enable move
    c_java_object ( const c_java_object & ) = delete;
    c_java_object & operator=( const c_java_object & ) = delete;

    c_java_object ( c_java_object && other ) noexcept
        : m_obj ( other.m_obj ), m_java ( other.m_java ),
        m_owns_ref ( other.m_owns_ref ) {
        other.m_obj = nullptr;
        other.m_owns_ref = false;
    }

    c_java_object & operator=( c_java_object && other ) noexcept {
        if ( this != &other ) {
            if ( m_owns_ref && m_obj && m_java ) {
            	m_java->get_env ( )->DeleteLocalRef ( m_obj );
            }
            m_obj = other.m_obj;
            m_java = other.m_java;
            m_owns_ref = other.m_owns_ref;
            other.m_obj = nullptr;
            other.m_owns_ref = false;
        }
        return *this;
    }

#ifdef _DEBUG
    void debug_print ( ) const {
        if ( !m_obj || !m_java ) {
            printf ( "debug_print: null object\n" );
            return;
        }

        JNIEnv * env = m_java->get_env ( );
        jclass obj_class = env->GetObjectClass ( m_obj );
        jclass class_class = env->FindClass ( "java/lang/Class" );
        jmethodID get_name = env->GetMethodID ( class_class, "getName", "()Ljava/lang/String;" );
        auto name_jstr = ( jstring ) env->CallObjectMethod ( obj_class, get_name );

        const char * name = env->GetStringUTFChars ( name_jstr, nullptr );
        printf ( "debug_print: %s\n", name );

        env->ReleaseStringUTFChars ( name_jstr, name );
        env->DeleteLocalRef ( name_jstr );
        env->DeleteLocalRef ( class_class );
        env->DeleteLocalRef ( obj_class );
    }
#endif

    [[nodiscard]] jobject get ( ) const { return m_obj; }
    [[nodiscard]] bool is_valid ( ) const { return m_obj != nullptr; }
    explicit operator bool ( ) const { return is_valid ( ); }
};