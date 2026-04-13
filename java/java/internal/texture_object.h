#pragma once
#include "abstract_texture.h"
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_texture_object : public c_java_object {
public:
    c_texture_object ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] int get_gl_texture_id ( ) const {
        const auto & cache = texture_object_cache::get ( m_java );
        return cache.get_gl_texture_id ? m_java->get_env ( )->CallIntMethod ( m_obj, cache.get_gl_texture_id ) : -1;
    }

    c_abstract_texture get_abstract_texture ( ) {
        return { m_obj, m_java, false };
    }
};