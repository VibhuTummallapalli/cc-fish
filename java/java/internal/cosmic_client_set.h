#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_cosmic_client_set : public c_java_object {
public:
    c_cosmic_client_set ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] int get_id ( ) const {
        const auto & cache = cosmic_client_set_cache::get ( m_java );
        return cache.get_id ? m_java->get_env ( )->CallIntMethod ( m_obj, cache.get_id ) : -1;
    }
};