#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_moving_object_type : public c_java_object {
public:
    c_moving_object_type ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

	 // returns hit type id, 0 = entity, 1 = block, 2 = miss
    [[nodiscard]] int get_id () const {
        const auto & cache = moving_object_type_cache::get ( m_java );
		return cache.get_id ? m_java->get_env ( )->CallIntMethod ( m_obj, cache.get_id ) : -1;
    }

    bool is_entity () const {
        return get_id ( ) == 2;
    }

    bool is_block () const {
        return get_id ( ) == 1;
    }
};