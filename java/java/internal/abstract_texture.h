#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_abstract_texture : public c_java_object {
public:
    c_abstract_texture ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] int get_texture_id ( ) const {
        const auto & cache = abstract_texture_cache::get ( m_java );
        return cache.gl_texture_id ? m_java->get_env ( )->GetIntField ( m_obj, cache.gl_texture_id ) : -1;
    }

    void set_texture_id ( const int val ) const {
        const auto & cache = abstract_texture_cache::get ( m_java );
        if ( cache.gl_texture_id ) m_java->get_env ( )->SetIntField ( m_obj, cache.gl_texture_id, val );
    }
};