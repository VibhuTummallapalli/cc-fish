#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_nbt_tag_compound : public c_java_object {
public:
    c_nbt_tag_compound ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] std::string get_string ( ) const {
        const auto & cache = nbt_tag_compound_cache::get ( m_java );
        if ( !cache.to_string || !m_obj ) return "";

        const auto jstr = static_cast < jstring > ( m_java->get_env ( )->CallObjectMethod ( m_obj, cache.to_string, static_cast < jchar > ( 0 ) ) );
        if ( !jstr ) return "";

        const char * str = m_java->get_env ( )->GetStringUTFChars ( jstr, nullptr );
        std::string result ( str );
        m_java->get_env ( )->ReleaseStringUTFChars ( jstr, str );
        m_java->get_env ( )->DeleteLocalRef ( jstr );
        return result;
    }
};