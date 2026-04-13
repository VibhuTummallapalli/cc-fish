#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_render_manager : public c_java_object {
public:
    c_render_manager ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] double get_render_pos_x ( ) const {
        const auto & cache = render_manager_cache::get ( m_java );
        return cache.render_pos_x ? m_java->get_env (  )->GetDoubleField ( m_obj, cache.render_pos_x ) : 0.0;
    }
    [[nodiscard]] double get_render_pos_y ( ) const {
        const auto & cache = render_manager_cache::get ( m_java );
        return cache.render_pos_y ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.render_pos_y ) : 0.0;
    }
    [[nodiscard]] double get_render_pos_z ( ) const {
        const auto & cache = render_manager_cache::get ( m_java );
        return cache.render_pos_z ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.render_pos_z ) : 0.0;
    }
    [[nodiscard]] double get_viewer_pos_x ( ) const {
        const auto & cache = render_manager_cache::get ( m_java );
        return cache.viewer_pos_x ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.viewer_pos_x ) : 0.0;
    }
    [[nodiscard]] double get_viewer_pos_y ( ) const {
        const auto & cache = render_manager_cache::get ( m_java );
        return cache.viewer_pos_y ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.viewer_pos_y ) : 0.0;
    }
    [[nodiscard]] double get_viewer_pos_z ( ) const {
        const auto & cache = render_manager_cache::get ( m_java );
        return cache.viewer_pos_z ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.viewer_pos_z ) : 0.0;
    }
    [[nodiscard]] vec3<double> get_render_pos ( ) const {
        return { get_render_pos_x ( ), get_render_pos_y ( ), get_render_pos_z ( ) };
	}
    [[nodiscard]] vec3<double> get_viewer_pos ( ) const {
        return { get_viewer_pos_x ( ), get_viewer_pos_y ( ), get_viewer_pos_z ( ) };
    }
};