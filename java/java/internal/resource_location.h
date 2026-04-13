#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_resource_location : public c_java_object {
public:
    c_resource_location ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] std::string get_resource_domain ( ) const {
        const auto & cache = resource_location_cache::get ( m_java );
        if ( !cache.resource_domain || !m_obj ) return "";

		const auto jstr = static_cast < jstring > ( m_java->get_env ( )->GetObjectField ( m_obj, cache.resource_domain ) );
        if ( !jstr ) return "";

        const char * str = m_java->get_env ( )->GetStringUTFChars ( jstr, nullptr );
        std::string result ( str );
        m_java->get_env ( )->ReleaseStringUTFChars ( jstr, str );
        m_java->get_env ( )->DeleteLocalRef ( jstr );
        return result;
    }

    [[nodiscard]] std::string get_resource_path ( ) const {
        const auto & cache = resource_location_cache::get ( m_java );
        if ( !cache.resource_path || !m_obj ) return "";

        const auto jstr = static_cast < jstring > ( m_java->get_env ( )->GetObjectField ( m_obj, cache.resource_path ) );
        if ( !jstr ) return "";

        const char * str = m_java->get_env ( )->GetStringUTFChars ( jstr, nullptr );
        std::string result ( str );
        m_java->get_env ( )->ReleaseStringUTFChars ( jstr, str );
        m_java->get_env ( )->DeleteLocalRef ( jstr );
        return result;
    }

    static c_resource_location create_resource_location ( const std::string & name, c_java * java ) {
        const auto & cache = resource_location_cache::get ( java );
        if ( !cache.init ) return { nullptr, java, false };
        const auto env = java->get_env ( );
        const jstring jname = env->NewStringUTF ( name.c_str ( ) );
        const jobject obj = env->NewObject ( cache.resource_location_class, cache.init, jname, 0, 0 );
        env->DeleteLocalRef ( jname );
        return { obj, java };
	}
};