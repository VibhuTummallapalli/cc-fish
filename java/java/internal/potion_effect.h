#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_potion_effect : public c_java_object {
public:
    c_potion_effect ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] int get_id ( ) const {
        const auto & cache = potion_effect_cache::get ( m_java );
        return cache.id ? m_java->get_env ( )->GetIntField ( m_obj, cache.id ) : -1;
    }
};