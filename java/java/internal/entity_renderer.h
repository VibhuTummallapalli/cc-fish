#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_entity_renderer : public c_java_object {
public:
    c_entity_renderer ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] float get_thirdperson_distance ( ) const {
        const auto & cache = entity_renderer_cache::get ( m_java );
        return cache.thirdperson_distance ? m_java->get_env (  )->GetFloatField ( m_obj, cache.thirdperson_distance ) : 4.0f;
    }

    void set_thirdperson_distance ( const float val ) const {
        const auto & cache = entity_renderer_cache::get ( m_java );
        if ( cache.thirdperson_distance ) m_java->get_env ( )->SetFloatField ( m_obj, cache.thirdperson_distance, val );
    }

    void call_get_mouse_over ( ) const {
        const auto & cache = entity_renderer_cache::get ( m_java );
		if ( cache.get_mouse_over ) m_java->get_env ( )->CallObjectMethod ( m_obj, cache.get_mouse_over, 1.0f, 0, 0 );
    }
};