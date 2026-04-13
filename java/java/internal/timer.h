#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_timer : public c_java_object {
public:
    c_timer ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] float get_render_partial_ticks ( ) const {
        const auto & cache = timer_cache::get ( m_java );
        return cache.render_partial_ticks ? m_java->get_env (  )->GetFloatField ( m_obj, cache.render_partial_ticks ) : 0.0f;
    }
};
