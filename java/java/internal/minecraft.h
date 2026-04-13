#pragma once
#include "active_render_info.h"
#include "entity.h"
#include "entity_renderer.h"
#include "game_settings.h"
#include "gui_inventory.h"
#include "mouse_helper.h"
#include "player_controller_mp.h"
#include "render_manager.h"
#include "texture_manager.h"
#include "timer.h"
#include "world.h"
#include "java_object/java_object.h"

class c_minecraft : public c_java_object {
public:
	explicit c_minecraft ( c_java * java ) : c_java_object ( java->get_minecraft ( ), java, false ) { }

	[[nodiscard]] c_entity get_player ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        const jobject player = cache.the_player ? m_java->get_env (  )->GetObjectField ( m_obj, cache.the_player ) : nullptr;
        return { player, m_java, true };
    }

	[[nodiscard]] c_world get_world ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        const jobject world = cache.the_world ? m_java->get_env (  )->GetObjectField ( m_obj, cache.the_world ) : nullptr;
        return { world, m_java, true };
	}

	[[nodiscard]] c_render_manager get_render_manager ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        const jobject rm = cache.get_render_manager ? m_java->get_env ( )->CallObjectMethod ( m_obj, cache.get_render_manager ) : nullptr;
        return { rm, m_java, true };
    }

	[[nodiscard]] c_entity_renderer get_entity_renderer ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        const jobject er = cache.entity_renderer ? m_java->get_env ( )->GetObjectField ( m_obj, cache.entity_renderer ) : nullptr;
        return { er, m_java, true };
    }

    [[nodiscard]] c_active_render_info get_active_render_info ( ) const {
        const auto & cache = active_render_info_cache::get ( m_java );
        const jobject ari = cache.active_render_info_class ? cache.active_render_info_class : nullptr;
        return { ari, m_java, true };
    }

	[[nodiscard]] c_game_settings get_game_settings ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        const jobject gs = cache.game_settings ? m_java->get_env ( )->GetObjectField ( m_obj, cache.game_settings ) : nullptr;
        return { gs, m_java, true };
    }

	[[nodiscard]] c_player_controller_mp get_player_controller ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        const jobject pc = cache.player_controller_mp ? m_java->get_env (  )->GetObjectField ( m_obj, cache.player_controller_mp ) : nullptr;
        return { pc, m_java, true };
	}

	[[nodiscard]] c_timer get_timer ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        const jobject timer = cache.get_timer ? m_java->get_env ( )->GetObjectField ( m_obj, cache.get_timer ) : nullptr;
        return { timer, m_java, true };
    }

    [[nodiscard]] c_mouse_helper get_mouse_helper ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        const jobject mh = cache.mouse_helper ? m_java->get_env ( )->GetObjectField ( m_obj, cache.mouse_helper ) : nullptr;
        return { mh, m_java, true };
    }

    [[nodiscard]] c_texture_manager get_texture_manager ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        const jobject tm = cache.get_texture_manager ? m_java->get_env ( )->CallObjectMethod ( m_obj, cache.get_texture_manager ) : nullptr;
        return { tm, m_java, true };
	}

	[[nodiscard]] c_moving_object_position get_object_mouse_over ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        const jobject om = cache.object_mouse_over ? m_java->get_env ( )->GetObjectField ( m_obj, cache.object_mouse_over ) : nullptr;
        return { om, m_java, true };
	}

	[[nodiscard]] bool get_in_game_has_focus ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        return cache.in_game_has_focus ? m_java->get_env ( )->GetBooleanField ( m_obj, cache.in_game_has_focus ) : false;
    }

    void set_in_game_has_focus ( const bool val ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        if ( cache.in_game_has_focus ) m_java->get_env ( )->SetBooleanField ( m_obj, cache.in_game_has_focus, val );
    }

	[[nodiscard]] int get_right_click_delay ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        return cache.right_click_delay ? m_java->get_env ( )->GetIntField ( m_obj, cache.right_click_delay ) : 0;
    }

    void set_right_click_delay ( const int val ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        if ( cache.right_click_delay ) m_java->get_env ( )->SetIntField ( m_obj, cache.right_click_delay, val );
    }

    void send_click ( ) const {
        const auto & cache = minecraft_cache::get ( m_java );
		if ( cache.send_click ) m_java->get_env ( )->CallVoidMethod ( m_obj, cache.send_click, 0, 0, 0, 0 );
	}

    void display_gui_screen ( c_entity * player_ent ) const {
        const auto & cache = minecraft_cache::get ( m_java );
        if ( cache.display_gui_screen ) {
            if ( player_ent ) {
                const auto & gui_inv_cache = gui_inventory_cache::get ( m_java );
                if ( !gui_inv_cache.gui_inventory_class || !gui_inv_cache.init ) return;
                auto gui = c_gui_inventory::create_inventory ( player_ent->get ( ), m_java );
                jobject gui_inv = gui.get ( );
                if ( gui_inv ) m_java->get_env ( )->CallVoidMethod ( m_obj, cache.display_gui_screen, gui_inv, 0, 0, 0 );
                //return;
            }
            // never get here, if we pass something thats null, we quit to main menu!
            //m_java->get_env ( )->CallVoidMethod ( m_obj, cache.display_gui_screen, gui_inv, 0, 0, 0 );
        }
    }

    void set_current_screen ( jobject obj ) const {
		const auto & cache = minecraft_cache::get ( m_java );
		if ( cache.current_screen ) m_java->get_env ( )->SetObjectField ( m_obj, cache.current_screen, obj );
	}
};