#pragma once
#include "entity.h"
#include "mc_vector.h"
#include "moving_object_type.h"
#include "java_object/java_object.h"

class c_moving_object_position : public c_java_object {
public:
    c_moving_object_position ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] c_entity get_entity_hit ( ) const {
        const auto & cache = moving_object_position_cache::get ( m_java );
		return cache.entity_hit ? c_entity ( m_java->get_env ( )->GetObjectField ( m_obj, cache.entity_hit ), m_java ) : c_entity ( nullptr, m_java, false );
    }
    [[nodiscard]] c_mc_vector get_hit_vec ( ) const {
        const auto & cache = moving_object_position_cache::get ( m_java );
		return cache.hit_vec ? c_mc_vector ( m_java->get_env ( )->GetObjectField ( m_obj, cache.hit_vec ), m_java ) : c_mc_vector ( nullptr, m_java, false );
    }
	[[nodiscard]] c_moving_object_type get_type_of_hit ( ) const {
        const auto & cache = moving_object_position_cache::get ( m_java );
        return cache.type_of_hit ? c_moving_object_type ( m_java->get_env ( )->GetObjectField ( m_obj, cache.type_of_hit ), m_java ) : c_moving_object_type ( nullptr, m_java, false );
	}
};