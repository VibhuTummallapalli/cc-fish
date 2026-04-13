#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_item : public c_java_object {
public:
    c_item ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] int get_id ( ) const {
        const auto & [item_class, get_id_from_item] = item_cache::get ( m_java );
        return get_id_from_item ? m_java->get_env (  )->CallStaticIntMethod ( item_class, get_id_from_item, m_obj ) : -1;
	}
};