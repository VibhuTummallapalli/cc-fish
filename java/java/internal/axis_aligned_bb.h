#pragma once
#include "../../math/vec.h"
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_axis_aligned_bb : public c_java_object {
public:
    c_axis_aligned_bb ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    // getters
    [[nodiscard]] double get_min_x ( ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        return cache.min_x ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.min_x ) : 0.0;
    }
    [[nodiscard]] double get_min_y ( ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        return cache.min_y ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.min_y ) : 0.0;
    }
    [[nodiscard]] double get_min_z ( ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        return cache.min_z ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.min_z ) : 0.0;
    }
    [[nodiscard]] double get_max_x ( ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        return cache.max_x ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.max_x ) : 0.0;
    }
    [[nodiscard]] double get_max_y ( ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        return cache.max_y ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.max_y ) : 0.0;
    }
    [[nodiscard]] double get_max_z ( ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        return cache.max_z ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.max_z ) : 0.0;
    }
    [[nodiscard]] vec3<double> get_mins ( ) const {
		return { get_min_x ( ), get_min_y ( ), get_min_z ( ) };
    }
    [[nodiscard]] vec3<double> get_maxs ( ) const { 
        return { get_max_x ( ), get_max_y ( ), get_max_z ( ) };
	}

    // setters
    void set_min_x ( const double val ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        if ( cache.min_x ) m_java->get_env ( )->SetDoubleField ( m_obj, cache.min_x, val );
    }
    void set_min_y ( const double val ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        if ( cache.min_y ) m_java->get_env ( )->SetDoubleField ( m_obj, cache.min_y, val );
    }
    void set_min_z ( const double val ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        if ( cache.min_z ) m_java->get_env ( )->SetDoubleField ( m_obj, cache.min_z, val );
    }
    void set_max_x ( const double val ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        if ( cache.max_x ) m_java->get_env ( )->SetDoubleField ( m_obj, cache.max_x, val );
    }
    void set_max_y ( const double val ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        if ( cache.max_y ) m_java->get_env ( )->SetDoubleField ( m_obj, cache.max_y, val );
    }
    void set_max_z ( const double val ) const {
        const auto & cache = axis_aligned_bb_cache::get ( m_java );
        if ( cache.max_z ) m_java->get_env ( )->SetDoubleField ( m_obj, cache.max_z, val );
    }
    void set_mins ( const vec3<double> & mins ) const {
        set_min_x ( mins.x );
        set_min_y ( mins.y );
        set_min_z ( mins.z );
	}
    void set_maxs ( const vec3<double> & maxs ) const {
        set_max_x ( maxs.x );
        set_max_y ( maxs.y );
        set_max_z ( maxs.z );
	}
};