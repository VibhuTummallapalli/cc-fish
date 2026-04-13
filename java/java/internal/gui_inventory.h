#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_gui_inventory : public c_java_object {
public:
    c_gui_inventory ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    static c_gui_inventory create_inventory ( jobject player_entity_in, c_java * java ) {
        const auto & cache = gui_inventory_cache::get ( java );
        if ( !cache.init ) return { nullptr, java };
        const jobject obj = java->get_env ( )->NewObject ( cache.gui_inventory_class,
            cache.init,
            player_entity_in
        );
        return { obj, java };
    }
};