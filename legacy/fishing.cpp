// c_fishing.cpp
#include "fishing.h"

#include "../../java/internal/entity.h"
#include "../../java/internal/native_bridge.h"
#include "../../main/hack.h"

void c_fishing::is_bobber ( const c_entity * entity, const c_entity * local_player ) {
    if ( !g_hack->vars.m_autofishing_enabled )
        return;

    if ( !entity || !entity->is_fishing_bobber (  ) )
        return;

    // If we have the local player, verify this bobber belongs to us
    // This prevents false positives when other players are fishing nearby
    if ( local_player && local_player->get ( ) ) {
        if ( !entity->is_bobber_owned_by ( local_player->get ( ) ) ) {
            return;  // This bobber belongs to another player, skip it
        }
    }

    auto uuid = entity->get_uuid ( ).get_value (  );
    auto pos = entity->get_pos ( );

    // update tracked position regardless
    m_tracked_bobbers [ uuid ] = pos;

    // if we don't have a bobber yet and we're expecting one,
    // claim the first fishing bobber that spawns near us
    if ( !m_has_our_bobber && m_first_bobber_pending ) {
        double dist_sq = distance_sq ( pos, m_local_pos_at_cast );
        // bobber spawns right next to the player, generous threshold
        if ( dist_sq < 6.0 * 6.0 ) {
            m_our_bobber_uuid = uuid;
            m_our_bobber_pos = pos;
            m_has_our_bobber = true;
            m_first_bobber_pending = false;
            m_state = e_state::waiting;
        }
        return;
    }

    // if this is our tracked bobber, keep its position fresh
    if ( m_has_our_bobber && uuid == m_our_bobber_uuid ) {
        m_our_bobber_pos = pos;
    }
}

void c_fishing::on_fish ( double x, double y, double z ) {
    // called from a different thread � just flag it if the
    // sound is close enough to our bobber
    if ( !m_has_our_bobber )
        return;

    double dx = x - m_our_bobber_pos.x;
    double dy = y - m_our_bobber_pos.y;
    double dz = z - m_our_bobber_pos.z;
    double dist_sq = dx * dx + dy * dy + dz * dz;

    if ( dist_sq <= g_hack->vars.m_sound_distance_threshold * g_hack->vars.m_sound_distance_threshold ) {
        m_fish_sound_pos = { x, y, z };
        m_reel_in.store ( true, std::memory_order_release );
    }
}

void c_fishing::update (
    c_entity * m_local,
    c_player_controller_mp * player_controller,
    c_world * m_world,
    c_itemstack * item_stack,
    int held_item_id
) {
    // this should always be valid when this is called
    const c_native_bridge native_bridge ( &g_hack->java );

    if ( m_last_fishing_state != g_hack->vars.m_autofishing_enabled ) {
        native_bridge.set_autofishing ( g_hack->vars.m_autofishing_enabled );
		m_last_fishing_state = g_hack->vars.m_autofishing_enabled;
    }

    if ( !g_hack->vars.m_autofishing_enabled )
        return;

    if ( !m_local || !player_controller || !m_world || !item_stack )
        return;
    if ( held_item_id != 346 ) { // not holding a fishing rod
        if ( m_state != e_state::idle ) {
            reset ( );
        }
        return;
	}

    auto now = clock_t::now ( );
    auto elapsed = std::chrono::duration<double> ( now - m_state_timestamp ).count ( );

    switch ( m_state ) {
    case e_state::idle: {
        // initial cast � record our position so we can identify
        // which bobber belongs to us when it spawns
        m_local_pos_at_cast = m_local->get_pos ( );
        m_first_bobber_pending = true;

        player_controller->use_item (
            m_local->get ( ), m_world->get ( ), item_stack->get ( )
        );

        m_state = e_state::waiting;
        m_state_timestamp = now;
        break;
    }

    case e_state::waiting: {
        // check the atomic flag set by on_fish from another thread
        if ( m_reel_in.load ( std::memory_order_acquire ) ) {
            m_state = e_state::reeling;
            m_state_timestamp = now;
        }
        break;
    }

    case e_state::reeling: {
        if ( elapsed < g_hack->vars.m_reel_delay_s )
            break;

        // reel in (right-click again pulls the bobber back)
        player_controller->use_item (
            m_local->get ( ), m_world->get ( ), item_stack->get ( )
        );

        m_reel_in.store ( false, std::memory_order_release );
        m_has_our_bobber = false;
        m_our_bobber_uuid = {};

        m_state = e_state::recasting;
        m_state_timestamp = now;
        break;
    }

    case e_state::recasting: {
        if ( elapsed < g_hack->vars.m_recast_delay_s )
            break;

        // cast again
        m_local_pos_at_cast = m_local->get_pos ( );
        m_first_bobber_pending = true;

        player_controller->use_item (
            m_local->get ( ), m_world->get ( ), item_stack->get ( )
        );

        m_state = e_state::waiting;
        m_state_timestamp = now;
        break;
    }
    }
}

void c_fishing::reset ( ) {
    m_tracked_bobbers.clear ( );
    m_our_bobber_uuid = {};
    m_our_bobber_pos = {};
    m_has_our_bobber = false;
    m_first_bobber_pending = false;
    m_reel_in.store ( false );
    m_state = e_state::idle;
    m_state_timestamp = {};
}