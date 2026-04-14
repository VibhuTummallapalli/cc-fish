#pragma once
#include <unordered_map>
#include <chrono>
#include <optional>
#include "module_utils.h"
#include "../java/internal/uuid.h"
#include "../../math/vec.h"
#include "../java/internal/cosmic_client_handler.h"
#include "aimassist/aimassist.h"
#include "autoclicker/autoclicker.h"
#include "pot_assist/pot_assist.h"
#include "cape/cape.h"
#include "esp/esp.h"
#include "fishing/fishing.h"
#include "misc_combat/misc_combat.h"
#include "pings/pings.h"
#include "potions/potions.h"
#include "reach/reach.h"
#include "selection/selection.h"
#include "trajectories/local_trajectories.h"
#include "trajectories/trajectory.h"
#include "velocity/velocity.h"
#include "world_modulation/world_modulation.h"

struct st_player_info {
    // Identity
    c_uuid::uuid_value uuid;
    std::string name;

    // Transform data
    vec3<double> position;
    vec3<double> eye_position;
    vec3<double> prev_position;
    vec3<double> mins;
    vec3<double> maxs;

    // Health and display
    float health = 0.f;
    float max_health = 0.f;
    float alpha = 1.f;
    int set = 0;

    // Timing
    long long last_tick = 0;
    long long last_held_pearl_tick = 0;
    long long last_held_potion_tick = 0;
    long long combat_tag_start_tick = 0;
    long long combat_tag_end_tick = 0;
    std::chrono::steady_clock::time_point last_seen;

    // State flags
    bool is_friend = false;
    bool is_dormant = false;
    bool is_local = false;
    bool hovered = false;
    bool invisible = false;

    // Pearl tracking
    size_t active_pearl_count = 0;
    long long soonest_pearl_spawn_tick = 0;
    long long soonest_pearl_land_tick = 0;
    bool on_pearl_cooldown = false;
    std::chrono::steady_clock::time_point pearl_cooldown_start;
    float pearl_cooldown_remaining = 0.f;

    // Potion tracking
    int potions_thrown = 0;
    int missed_potions = 0;
    int cur_potions_thrown = 0;
    int cur_missed_potions = 0;
    double potion_efficiency = 0.0;

    st_player_info ( ) = default;

    explicit st_player_info ( c_uuid::uuid_value uuid )
        : uuid ( uuid )
        , last_seen ( std::chrono::steady_clock::now ( ) ) {
    }

    void update_transform ( const vec3<double> & pos, const vec3<double> & eye_pos,
        const vec3<double> & prev_pos, const vec3<double> & min_bounds,
        const vec3<double> & max_bounds, long long cur_tick,
        float cur_health, float cur_max_health, int current_set,
        bool is_invisible );

    void set_dormant ( bool dormant ) { is_dormant = dormant; }
    void set_friend ( bool friend_status ) { is_friend = friend_status; }
    void toggle_friend ( ) { is_friend = !is_friend; }

    // Pearl management
    void set_last_held_pearl_tick ( long long tick ) { last_held_pearl_tick = tick; }
    void update_pearl_cache ( size_t count, long long spawn_tick, long long land_tick );
    void start_pearl_cooldown ( float duration_seconds );
    void update_pearl_cooldown ( float total_duration_seconds );
    [[nodiscard]] bool has_active_pearls ( ) const { return active_pearl_count > 0; }

    // Combat tracking
    void set_combat_tag_ticks ( long long start_tick, long long end_tick );

    // Potion management
    void set_last_held_potion_tick ( long long tick ) { last_held_potion_tick = tick; }
    void update_potion_cache ( int count, int missed, double efficiency );
    void increment_potion_count ( );
    void reset_potion_count ( );
    [[nodiscard]] bool has_thrown_potions ( ) const { return cur_potions_thrown > 0; }
    [[nodiscard]] int get_missed_potions ( ) const { return cur_missed_potions; }
};

class c_module_handler {
public:
    void initialize ( );
    void update ( );
    void clear_cache ( );

    [[nodiscard]] auto & get_player_cache ( ) { return m_player_cache; }
    [[nodiscard]] std::optional<st_player_info> get_player_info ( const c_uuid::uuid_value & uuid ) const;
    [[nodiscard]] c_module_utils & get_utils ( ) { return m_utils; }
    [[nodiscard]] const c_minecraft & get_minecraft ( ) const { return *m_minecraft; }
    [[nodiscard]] c_trajectories & get_trajectories ( ) { return m_trajectories; }
    [[nodiscard]] c_cape & get_cape ( ) { return m_cape; }
	[[nodiscard]] c_pings & get_pings ( ) { return m_pings; }
	[[nodiscard]] c_autoclicker & get_autoclicker ( ) { return m_autoclicker; }
	[[nodiscard]] c_aimassist & get_aimassist ( ) { return m_aimassist; }
	[[nodiscard]] c_fishing & get_fishing ( ) { return m_fishing; }

    static bool world_changed ( const jobject & world );

private:
    // Player tracking
    std::unordered_map<c_uuid::uuid_value, st_player_info, c_uuid::uuid_value::hash> m_player_cache;
    long long m_last_tick = 0;
    bool m_was_ingame = false;

    // Modules
    c_module_utils m_utils;
    c_esp m_esp;
    c_trajectories m_trajectories;
    c_cape m_cape;
    c_selection m_selection;
    c_aimassist m_aimassist;
    c_potions m_potions;
    c_pings m_pings;
    c_world_modulation m_world_modulation;
	c_reach m_reach;
    c_local_trajectories m_local_trajectories;
    c_misc_combat m_misc_combat;
    c_velocity m_velocity;
    c_autoclicker m_autoclicker;
    c_pot_assist m_pot_assist;
    c_fishing m_fishing;

    std::optional<c_minecraft> m_minecraft;
    std::optional<c_cosmic_client_handler> m_cosmic_client;
    static jobject m_world;

    // Helper methods
    void setup_callbacks ( );
    void process_entities ( c_world & world, long long cur_tick, int local_entity_id, const c_entity * local_player );
    void process_players ( c_world & world, const c_texture_manager & texture_manager,
        long long cur_tick, int local_entity_id );
    void handle_player_selection ( const c_entity & local_player, c_world & world );
};