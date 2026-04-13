#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_game_settings : public c_java_object {
public:
    c_game_settings ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] float get_fov ( ) const {
        const auto & cache = game_settings_cache::get ( m_java );
        return cache.fov ? m_java->get_env (  )->GetFloatField ( m_obj, cache.fov ) : 90.0f;
    }

    void set_fov ( const float val ) const {
        const auto & cache = game_settings_cache::get ( m_java );
        if ( cache.fov ) m_java->get_env ( )->SetFloatField ( m_obj, cache.fov, val );
    }
};