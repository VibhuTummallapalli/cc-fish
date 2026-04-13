#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_player_controller_mp : public c_java_object {
public:
    c_player_controller_mp ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    void window_click ( const int window_id, const int slot, const int mouse_button, const int mode, const jobject player ) const {
        const auto & cache = player_controller_mp_cache::get ( m_java );
        if ( cache.window_click ) m_java->get_env (  )->CallObjectMethod ( m_obj, cache.window_click, window_id, slot, mouse_button, mode, player );
    }

    void use_item ( const jobject player, const jobject world, const jobject item_stack ) const {
        const auto & cache = player_controller_mp_cache::get ( m_java );
        if ( cache.send_use_item ) m_java->get_env (  )->CallObjectMethod ( m_obj, cache.send_use_item, player, world, item_stack );
	}

    void attack_entity ( const jobject local_player, const jobject entity ) const {
        const auto & cache = player_controller_mp_cache::get ( m_java );
        if ( cache.attack_entity ) m_java->get_env ( )->CallObjectMethod ( m_obj, cache.attack_entity, static_cast < jshort > ( 0 ), static_cast < jshort > ( 0 ), static_cast < jint > ( 0 ), local_player, entity );
    }
};