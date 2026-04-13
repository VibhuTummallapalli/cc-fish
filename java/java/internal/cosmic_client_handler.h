#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"
#include "cosmic_packet_handler.h"

class c_cosmic_client_handler : public c_java_object {
public:
    explicit c_cosmic_client_handler ( c_java * java ) : c_java_object ( java->get_cosmic_client ( ), java, false ) { }

    [[nodiscard]] c_cosmic_packet_handler get_packet_handler ( ) const {
        const auto & cache = cosmic_client_handler_cache::get ( m_java );
		const jobject cph = cache.get_packet_handler ? m_java->get_env ( )->CallObjectMethod ( m_obj, cache.get_packet_handler ) : nullptr;
        return { cph, m_java };
	}
};