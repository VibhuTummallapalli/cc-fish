#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_block : public c_java_object {
public:
    c_block ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] int get_id ( ) const {
        const auto & cache = block_cache::get ( m_java );
        return cache.get_id_from_block ? m_java->get_env ( )->CallStaticIntMethod ( cache.block_class, cache.get_id_from_block, m_obj ) : 0;
    }

    [[nodiscard]] double get_max_x ( ) const {
        const auto & cache = block_cache::get ( m_java );
		return cache.max_x ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.max_x ) : 0.0;
    }
    [[nodiscard]] double get_max_y ( ) const {
		const auto & cache = block_cache::get ( m_java );
		return cache.max_y ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.max_y ) : 0.0;
    }
	[[nodiscard]] double get_max_z ( ) const {
        const auto & cache = block_cache::get ( m_java );
        return cache.max_z ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.max_z ) : 0.0;
    }
    [[nodiscard]] double get_min_x ( ) const {
        const auto & cache = block_cache::get ( m_java );
        return cache.min_x ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.min_x ) : 0.0;
    }
    [[nodiscard]] double get_min_y ( ) const {
        const auto & cache = block_cache::get ( m_java );
        return cache.min_y ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.min_y ) : 0.0;
    }
    [[nodiscard]] double get_min_z ( ) const {
        const auto & cache = block_cache::get ( m_java );
        return cache.min_z ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.min_z ) : 0.0;
    }
	[[nodiscard]] vec3 < double > get_maxs ( ) const {
        return { get_max_x ( ), get_max_y ( ), get_max_z ( ) };
    }
    [[nodiscard]] vec3 < double > get_mins ( ) const {
        return { get_min_x ( ), get_min_y ( ), get_min_z ( ) };
	}
};