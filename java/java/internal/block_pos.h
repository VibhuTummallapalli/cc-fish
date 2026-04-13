#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_block_pos : public c_java_object {
public:
    c_block_pos ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    static c_block_pos create_block_pos ( const double x, const double y, const double z, c_java * java ) {
        const auto & cache = block_pos_cache::get ( java );
        if ( !cache.init ) return { nullptr, java };
        const jobject obj = java->get_env ( )->NewObject ( cache.block_pos_class, cache.init, static_cast < jdouble > ( x ), static_cast < jdouble > ( y ), static_cast < jdouble > ( z ) );
        return { obj, java };
	}
};