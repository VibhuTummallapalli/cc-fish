#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_cosmic_ping : public c_java_object {
public:
    c_cosmic_ping ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    static c_cosmic_ping create_cosmic_ping ( const double x, const double y, const double z, const int dimension, const bool allies, c_java * java ) {
        const auto & cache = cosmic_ping_cache::get ( java );
        if ( !cache.init ) return { nullptr, java };
        const jobject obj = java->get_env ( )->NewObject ( cache.cosmic_ping_class, 
            cache.init, 
            static_cast < jdouble > ( x ), 
            static_cast < jdouble > ( y ), 
            static_cast < jdouble > ( z ),
            static_cast < jint > ( dimension ), 
            static_cast < jboolean > ( allies ) 
        );
        return { obj, java };
    }
};