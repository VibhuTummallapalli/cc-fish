#pragma once
#include "cosmic_ping.h"
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_cosmic_packet_handler : public c_java_object {
public:
    c_cosmic_packet_handler ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    void send_ping_packet ( const double x, const double y, const double z, const int dimension, const bool allies ) const {
        const auto & cache = cosmic_packet_handler_cache::get ( m_java );
        if ( cache.send_packet ) {
			const auto cosmic_ping = c_cosmic_ping::create_cosmic_ping ( x, y, z, dimension, allies, m_java );
            m_java->get_env ( )->CallVoidMethod ( m_obj, cache.send_packet, cosmic_ping.get ( ) );
        }
    }
};