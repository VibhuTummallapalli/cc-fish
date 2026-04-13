// c_fishing.h
#pragma once

#include <unordered_map>
#include <atomic>
#include <chrono>
#include "../../math/vec.h"
#include "../../java/internal/uuid.h"

class c_itemstack;
class c_world;
class c_player_controller_mp;
class c_entity;

class c_fishing {
public:
    void update (
        c_entity * m_local,
        c_player_controller_mp * player_controller,
        c_world * m_world,
        c_itemstack * item_stack,
        int held_item_id
    );

    void is_bobber ( const c_entity * entity );
    void on_fish ( double x, double y, double z );

    void reset ( );
private:
    enum class e_state {
        idle,
        waiting,
        reeling,
        recasting
    };

    struct bobber_data {
        c_uuid::uuid_value uuid;
        vec3<double> position;
    };

    bool m_last_fishing_state = false;

    c_uuid::uuid_value m_our_bobber_uuid {};
    std::unordered_map<c_uuid::uuid_value, vec3<double>, c_uuid::uuid_value::hash> m_tracked_bobbers;
    vec3<double> m_our_bobber_pos {};
    bool m_has_our_bobber = false;

    std::atomic<bool> m_reel_in { false };
    vec3<double> m_fish_sound_pos {};

    e_state m_state = e_state::idle;

    using clock_t = std::chrono::steady_clock;
    clock_t::time_point m_state_timestamp {};

    vec3<double> m_local_pos_at_cast {};
    bool m_first_bobber_pending = false;

    double distance_sq ( const vec3<double> & a, const vec3<double> & b ) const {
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        double dz = a.z - b.z;
        return dx * dx + dy * dy + dz * dz;
    }
};