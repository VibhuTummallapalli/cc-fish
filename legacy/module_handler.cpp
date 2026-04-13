#include "module_handler.h"
#include <ranges>
#include "../java/internal/minecraft.h"
#include "../main/hack.h"

jobject c_module_handler::m_world = nullptr;

// st_player_info implementations
void st_player_info::update_transform ( const vec3<double> & pos, const vec3<double> & eye_pos,
    const vec3<double> & prev_pos, const vec3<double> & min_bounds,
    const vec3<double> & max_bounds, long long cur_tick,
    float cur_health, float cur_max_health, int current_set,
    bool is_invisible ) {
    position = pos;
    eye_position = eye_pos;
    prev_position = prev_pos;
    mins = min_bounds;
    maxs = max_bounds;
    last_tick = cur_tick;
    last_seen = std::chrono::steady_clock::now ( );
    health = cur_health;
    max_health = cur_max_health;
    is_dormant = false;
    set = current_set;
    invisible = is_invisible;
}

void st_player_info::set_combat_tag_ticks ( long long start_tick, long long end_tick ) {
    combat_tag_start_tick = start_tick;
    combat_tag_end_tick = end_tick;
}

void st_player_info::update_pearl_cache ( size_t count, long long spawn_tick, long long land_tick ) {
    active_pearl_count = count;
    soonest_pearl_spawn_tick = ( count > 0 ) ? spawn_tick : 0;
    soonest_pearl_land_tick = ( count > 0 ) ? land_tick : 0;
}

void st_player_info::start_pearl_cooldown ( float duration_seconds ) {
    on_pearl_cooldown = true;
    pearl_cooldown_start = std::chrono::steady_clock::now ( );
    pearl_cooldown_remaining = duration_seconds;
}

void st_player_info::update_pearl_cooldown ( float total_duration_seconds ) {
    if ( !on_pearl_cooldown ) return;

    const auto now = std::chrono::steady_clock::now ( );
    const float elapsed = std::chrono::duration<float> ( now - pearl_cooldown_start ).count ( );
    pearl_cooldown_remaining = total_duration_seconds - elapsed;

    if ( pearl_cooldown_remaining <= 0.f ) {
        on_pearl_cooldown = false;
        pearl_cooldown_remaining = 0.f;
    }
}

void st_player_info::update_potion_cache ( int count, int missed, double efficiency ) {
    int original_missed = missed_potions;
    potions_thrown = count;
    missed_potions = missed;
    potion_efficiency = efficiency;
    cur_missed_potions += missed_potions - original_missed;
}

void st_player_info::increment_potion_count ( ) {
    potions_thrown++;
    cur_potions_thrown++;
}

void st_player_info::reset_potion_count ( ) {
    cur_potions_thrown = 0;
    cur_missed_potions = 0;
}

// c_module_handler implementations
void c_module_handler::setup_callbacks ( ) {
    // Pearl cache callback
    m_trajectories.set_pearl_cache_callback (
        [ this ] ( const c_uuid::uuid_value & player_uuid, size_t count,
            long long spawn_tick, long long land_tick ) {
                if ( auto it = m_player_cache.find ( player_uuid ); it != m_player_cache.end ( ) ) {
                    it->second.update_pearl_cache ( count, spawn_tick, land_tick );
                }
        }
    );

    // Pearl thrown callback
    m_trajectories.set_pearl_thrown_callback (
        [ this ] ( const c_uuid::uuid_value & player_uuid, bool is_friend ) {
            if ( auto it = m_player_cache.find ( player_uuid ); it != m_player_cache.end ( ) ) {
                const float cooldown_duration = is_friend
                    ? g_hack->vars.m_friend_pearl_cooldown_duration
                    : g_hack->vars.m_enemy_pearl_cooldown_duration;
                it->second.start_pearl_cooldown ( cooldown_duration );
            }
        }
    );

    // Potion thrown callback
    m_trajectories.set_potion_thrown_callback (
        [ this ] ( const c_uuid::uuid_value & player_uuid ) {
            if ( auto it = m_player_cache.find ( player_uuid ); it != m_player_cache.end ( ) ) {
                it->second.increment_potion_count ( );
            }
        }
    );

    // Potion efficiency callback
    m_trajectories.set_potion_efficiency_callback (
        [ this ] ( const c_uuid::uuid_value & player_uuid, int count, int missed, double efficiency ) {
            if ( auto it = m_player_cache.find ( player_uuid ); it != m_player_cache.end ( ) ) {
                it->second.update_potion_cache ( count, missed, efficiency );
            }
        }
    );
}

void c_module_handler::initialize ( ) {
    m_minecraft.emplace ( &g_hack->java );
    m_cosmic_client.emplace ( &g_hack->java );
    setup_callbacks ( );
}

void c_module_handler::process_entities ( c_world & world, long long cur_tick, int local_entity_id ) {
    const auto entities = world.get_entities ( );
    for ( const auto & entity : entities ) {
        if ( !entity.is_valid ( ) || entity.get_entity_id ( ) == local_entity_id ) {
            continue;
        }
        m_trajectories.process_entity ( entity, cur_tick );

        m_fishing.is_bobber ( &entity );
    }
}

void c_module_handler::process_players ( c_world & world, const c_texture_manager & texture_manager,
    long long cur_tick, int local_entity_id ) {
    const auto players = world.get_player_entities ( );

    for ( const auto & player : players ) {
        if ( !player.is_valid ( ) || !player.is_player ( ) ) {
            continue;
        }

        const auto player_uuid = player.get_uuid ( );
        if ( !player_uuid.is_valid ( ) ) {
            continue;
        }

        const auto uuid_value = player_uuid.get_value ( );
        auto [it, inserted] = m_player_cache.try_emplace ( uuid_value, uuid_value );

        // Initialize new player
        if ( inserted ) {
            it->second.name = player.get_name ( );
            it->second.is_local = player.get_entity_id ( ) == local_entity_id;

            g_hack->chams.add_player (
                uuid_value,
                it->second.is_local ? player_type::local :
                it->second.is_friend ? player_type::friendly :
                player_type::enemy
            );
        }

        // Update transform
        const auto bounding_box = player.get_bounding_box ( );
        it->second.update_transform (
            player.get_pos ( ), player.get_eye_pos ( ), player.get_last_pos ( ),
            bounding_box.get_mins ( ), bounding_box.get_maxs ( ),
            cur_tick, player.get_health ( ), player.get_max_health ( ),
            player.get_armor_configuration ( ), player.is_invisible ( )
        );

        // Combat tag tracking
        if ( player.get_hurt_time ( ) > 0 ) {
            const float tag_duration = it->second.is_friend
                ? g_hack->vars.m_friend_combat_tag_duration
                : g_hack->vars.m_enemy_combat_tag_duration;

            it->second.set_combat_tag_ticks (
                cur_tick,
                cur_tick + static_cast< long long >( 20.f * tag_duration )
            );
        }

        // Track held items
        if ( auto held_item = player.get_held_item ( ); held_item.is_valid ( ) ) {
            if ( auto item_stack = held_item.get_item ( ); item_stack.is_valid ( ) ) {
                const int item_id = item_stack.get_id ( );
                if ( item_id == 368 ) { // Ender pearl
                    it->second.set_last_held_pearl_tick ( cur_tick );
                }
                else if ( item_id == 373 ) { // Potion
                    it->second.set_last_held_potion_tick ( cur_tick );
                }
            }
        }

        // Classify projectiles
        m_trajectories.classify_projectiles (
            it->second.eye_position, uuid_value, it->second.name,
            it->second.is_local, it->second.is_friend, cur_tick,
            it->second.last_held_pearl_tick, it->second.last_held_potion_tick
        );
    }
}

void c_module_handler::handle_player_selection ( const c_entity & local_player, c_world & world ) {
    m_selection.begin_selection ( {
        local_player.get_pos ( ),
        local_player.get_eye_pos ( ),
        local_player.get_angles ( ),
        g_hack->vars.m_aim_assist_fov,
        25.f,
        g_hack->vars.m_aim_assist_max_distance,
        g_hack->vars.m_target_priority_order,
        g_hack->vars.m_aim_assist_target_through_walls,
        g_hack->vars.m_ignore_invisible_players
        }, &world );

    const float dt = g_hack->renderer.get_deltatime ( );
    const auto cur_tick = world.get_total_world_time ( );

    for ( auto & player_info : m_player_cache | std::views::values ) {
        if ( player_info.is_local ) continue;

        m_esp.update ( player_info, cur_tick, dt );
        m_selection.process_player_for_selection ( player_info );
        m_selection.process_player_for_hover ( player_info );
    }

    m_selection.end_selection ( );

    // Handle friending
    if ( auto hovered = m_selection.get_hovered_player ( ); hovered ) {
        if ( g_hack->vars.m_friend_key.active ) {
            const float distance = hovered->position.distance_to ( m_utils.get_local_pos ( ) );
            if ( distance <= static_cast < float > ( g_hack->vars.m_friending_max_distance ) ) {
                hovered->is_friend = !hovered->is_friend;

                g_hack->chams.update_player_type (
                    hovered->uuid,
                    hovered->is_friend ? player_type::friendly :
                    player_type::enemy
				);
            }
        }
    }

    // Handle aim assist
    const auto selected = m_selection.get_best_selected_player ( );

    if ( selected ) {
        m_aimassist.update ( selected, local_player );
    }

    m_velocity.update ( selected );
}

void c_module_handler::update ( ) {
    auto & mc = *m_minecraft;
	auto & cc = *m_cosmic_client;

    // Check if we're in-game
    auto world = mc.get_world ( );
    if ( !world.is_valid ( ) ) {
        if ( m_was_ingame ) {
            clear_cache ( );
            m_was_ingame = false;
        }
        return;
    }

    m_was_ingame = true;

    if ( world_changed ( world.get ( ) ) ) {
        clear_cache ( );
    }

    auto texture_manager = mc.get_texture_manager ( );
    if ( !texture_manager.is_valid ( ) ) {
        return;
    }

    // Update utilities
    m_utils.update ( mc );

    auto local_player = mc.get_player ( );
    const auto local_entity_id = local_player.get_entity_id ( );
    m_utils.set_local_pos ( local_player.get_pos ( ) );

    const auto cur_tick = world.get_total_world_time ( );

    // lagback on key
    if ( g_hack->vars.m_lagback_key.active ) {
        local_player.double_jump ( );
    }

    // Process tick-based updates
    if ( cur_tick != m_last_tick ) {
        auto player_controller = mc.get_player_controller ( );
        auto local_inventory = local_player.get_inventory ( );

        if ( !player_controller.is_valid ( ) || !local_inventory.is_valid ( ) ) {
            return;
        }

        // Update modules
        m_cape.update ( local_player, texture_manager );
        m_potions.update ( &local_player, &player_controller, &local_inventory, &world, &mc, *g_hack->input_system.get_window (  ) );
        m_pings.update ( &cc );
		m_world_modulation.update ( );
		m_reach.update ( );
        m_misc_combat.update ( );
        m_autoclicker.run ( local_player, mc );

        int local_item_id = -1;
        c_itemstack held_item = { nullptr, nullptr };

        // what is the local player holding
        if ( held_item = local_player.get_held_item ( ); held_item.is_valid ( ) ) {
            if ( const auto item_stack = held_item.get_item ( ); item_stack.is_valid ( ) ) {
                local_item_id = item_stack.get_id ( );
            }
        }

        // Update local trajectories
        m_local_trajectories.update ( &world, local_player, local_entity_id, local_item_id );
        m_pot_assist.update ( local_player, local_item_id );

        // fishing
        m_fishing.update ( &local_player, &player_controller, &world, &held_item, local_item_id);

        // Process entities and players
        process_entities ( world, cur_tick, local_entity_id );
        process_players ( world, texture_manager, cur_tick, local_entity_id );

        m_trajectories.finalize_classifications ( );

        // Build player position map for potion efficiency
        std::unordered_map<c_uuid::uuid_value, vec3<double>, c_uuid::uuid_value::hash> player_positions;
        for ( const auto & [uuid, player_info] : m_player_cache ) {
            player_positions [ uuid ] = player_info.position;
        }

        // Check landed projectiles
        m_trajectories.check_landed_projectiles ( cur_tick, player_positions );
        m_trajectories.cleanup_expired_projectiles ( );

        m_last_tick = cur_tick;
    }

    // Render trajectories
    m_trajectories.render_trajectories (
        m_utils.get_projection_matrix ( ),
        m_utils.get_model_view_matrix ( ),
        m_utils.get_render_pos ( )
    );

	// Render local trajectories
    m_local_trajectories.render (
        m_utils.get_projection_matrix ( ),
        m_utils.get_model_view_matrix ( ),
        m_utils.get_render_pos ( ),
        g_hack->vars.m_local_trajectory_color
    );

    // Render pot assist
    m_pot_assist.render ( local_player, m_utils );

    // Handle player selection and rendering
    handle_player_selection ( local_player, world );
}

bool c_module_handler::world_changed ( const jobject & world ) {
    if ( !g_hack->java.get_env ( )->IsSameObject ( world, m_world ) ) {
        if ( m_world ) {
            g_hack->java.get_env ( )->DeleteGlobalRef ( m_world );
        }
        m_world = g_hack->java.get_env ( )->NewGlobalRef ( world );
        return true;
    }
    return false;
}

void c_module_handler::clear_cache ( ) {
    for ( auto & player : m_player_cache | std::views::values ) {
        player.set_dormant ( true );
        player.last_tick = 0;
    }
    m_trajectories.clear ( );
    m_fishing.reset ( );
}

std::optional<st_player_info> c_module_handler::get_player_info ( const c_uuid::uuid_value & uuid ) const {
    if ( auto it = m_player_cache.find ( uuid ); it != m_player_cache.end ( ) ) {
        return it->second;
    }
    return std::nullopt;
}