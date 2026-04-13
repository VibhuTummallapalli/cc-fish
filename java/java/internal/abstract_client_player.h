#pragma once
#include "resource_location.h"
#include "java_object/java_object.h"

class c_abstract_client_player : public c_java_object {
public:
    c_abstract_client_player ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] c_resource_location get_location_skin ( ) const {
        const auto & cache = abstract_client_player_cache::get ( m_java );
        const jobject ls = cache.get_location_skin ? m_java->get_env ( )->CallObjectMethod ( m_obj, cache.get_location_skin ) : nullptr;
        return { ls, m_java };
    }

    [[nodiscard]] c_resource_location get_location_cape ( ) const {
        const auto & cache = abstract_client_player_cache::get ( m_java );
        const jobject lc = cache.get_location_cape ? m_java->get_env ( )->CallObjectMethod ( m_obj, cache.get_location_cape ) : nullptr;
        return { lc, m_java };
    }
};