#pragma once
#include "../../../java/java.h"
#include "../../../java/internal/minecraft.h"
#include "../../../java/internal/native_bridge.h"
#include "../../../utils/file_logging.h"
#include <chrono>
#include <atomic>
#include <thread>
#include <mutex>
#include <cmath>

// Cached bobber information
struct bobber_info {
    bool valid = false;
    long long uuid_most = 0;
    long long uuid_least = 0;
    double pos_x = 0.0;
    double pos_y = 0.0;
    double pos_z = 0.0;
};

class c_fishing {
private:
    c_java * m_java;
    std::atomic<bool> m_enabled { false };
    std::atomic<bool> m_auto_recast { true };
    std::atomic<int> m_recast_delay_ms { 500 };  // ms delay before recasting
    std::atomic<int> m_reel_delay_ms { 100 };    // ms delay before reeling in
    std::atomic<double> m_max_bobber_distance { 64.0 };  // max distance to consider bobber as ours
    std::atomic<double> m_sound_tolerance { 1.0 };  // tolerance for sound location vs bobber position (increased from 0.2)
    std::chrono::steady_clock::time_point m_last_catch_time;
    
    // Pending actions (set from callbacks, processed from main loop)
    std::atomic<bool> m_pending_reel { false };
    std::atomic<bool> m_pending_recast { false };
    std::chrono::steady_clock::time_point m_recast_time;
    
    // Cached bobber data
    bobber_info m_cached_bobber;
    mutable std::mutex m_bobber_mutex;
    bool m_last_had_bobber { false };  // For tracking state changes

public:
    explicit c_fishing ( c_java * java ) : m_java ( java ) { }

    // Update/scan for our fishing bobber in the world
    void update_bobber ( ) {
        LOG_INFO ( "update_bobber: start" );
        
        if ( !m_java ) {
            LOG_ERROR ( "update_bobber: m_java is null" );
            return;
        }

        LOG_INFO ( "update_bobber: creating minecraft instance" );
        c_minecraft minecraft ( m_java );
        
        LOG_INFO ( "update_bobber: getting player" );
        c_entity player = minecraft.get_player ( );
        
        LOG_INFO ( "update_bobber: getting world" );
        c_world world = minecraft.get_world ( );

        if ( !player.get ( ) ) {
            LOG_WARNING ( "update_bobber: player is null" );
            std::lock_guard<std::mutex> lock ( m_bobber_mutex );
            m_cached_bobber.valid = false;
            return;
        }
        
        if ( !world.get ( ) ) {
            LOG_WARNING ( "update_bobber: world is null" );
            std::lock_guard<std::mutex> lock ( m_bobber_mutex );
            m_cached_bobber.valid = false;
            return;
        }

        LOG_INFO ( "update_bobber: getting player position" );
        // Get player position
        double player_x = player.get_pos_x ( );
        double player_y = player.get_pos_y ( );
        double player_z = player.get_pos_z ( );
        LOG_INFO ( "update_bobber: player at " << player_x << ", " << player_y << ", " << player_z );

        // Check if EntityFishHook class is cached
        jclass fish_hook_class = m_java->get_class ( "EntityFishHook" );
        if ( !fish_hook_class ) {
            LOG_ERROR ( "update_bobber: EntityFishHook class not cached!" );
            return;
        }
        LOG_INFO ( "update_bobber: EntityFishHook class OK" );

        LOG_INFO ( "update_bobber: getting entities" );
        // Loop through all entities to find our bobber
        std::vector<c_entity> entities = world.get_entities ( );
        LOG_INFO ( "update_bobber: got " << entities.size ( ) << " entities" );
        
        // Push a local frame to handle many local refs
        JNIEnv * env = m_java->get_env ( );
        if ( env->PushLocalFrame ( 256 ) < 0 ) {
            LOG_ERROR ( "update_bobber: failed to push local frame" );
            return;
        }
        
        bool found_bobber = false;
        int entity_count = 0;
        for ( auto & entity : entities ) {
            entity_count++;
            
            // Clear any pending JNI exceptions
            if ( env->ExceptionCheck ( ) ) {
                env->ExceptionClear ( );
                continue;
            }
            
            try {
                if ( !entity.get ( ) ) continue;
                
                // Check if this entity is a fishing bobber - wrap in try/catch
                bool is_bobber = false;
                try {
                    is_bobber = entity.is_fishing_bobber ( );
                } catch ( ... ) {
                    continue;  // Skip entities that cause issues
                }
                
                if ( !is_bobber ) continue;
                
                LOG_INFO ( "update_bobber: found fishing bobber entity #" << entity_count );

                // Get bobber position
                double bobber_x = entity.get_pos_x ( );
                double bobber_y = entity.get_pos_y ( );
                double bobber_z = entity.get_pos_z ( );

                // Calculate distance from player to bobber
                double dx = bobber_x - player_x;
                double dy = bobber_y - player_y;
                double dz = bobber_z - player_z;
                double distance = std::sqrt ( dx * dx + dy * dy + dz * dz );

                // Check if bobber is within reasonable distance (our bobber)
                if ( distance > m_max_bobber_distance.load ( ) ) continue;

                // Get UUID of this bobber
                c_uuid uuid = entity.get_uuid ( );
                if ( !uuid.get ( ) ) continue;

                long long uuid_most = uuid.get_most_significant_bits ( );
                long long uuid_least = uuid.get_least_significant_bits ( );

                // Cache the bobber info
                {
                    std::lock_guard<std::mutex> lock ( m_bobber_mutex );
                    m_cached_bobber.valid = true;
                    m_cached_bobber.uuid_most = uuid_most;
                    m_cached_bobber.uuid_least = uuid_least;
                    m_cached_bobber.pos_x = bobber_x;
                    m_cached_bobber.pos_y = bobber_y;
                    m_cached_bobber.pos_z = bobber_z;
                }

                found_bobber = true;
                
                // Log when we start tracking a new bobber
                if ( !m_last_had_bobber ) {
                    LOG_SUCCESS ( "Tracking bobber at: " << bobber_x << ", " << bobber_y << ", " << bobber_z );
                    m_last_had_bobber = true;
                }
                
                break;  // Found our bobber, stop searching
            } catch ( ... ) {
                // Skip problematic entities
                continue;
            }
        }

        // If no bobber found, invalidate cache
        if ( !found_bobber ) {
            std::lock_guard<std::mutex> lock ( m_bobber_mutex );
            if ( m_last_had_bobber ) {
                LOG_WARNING ( "Bobber no longer detected" );
                m_last_had_bobber = false;
            }
            m_cached_bobber.valid = false;
        }
        
        // Pop the local frame
        env->PopLocalFrame ( nullptr );
        
        LOG_INFO ( "update_bobber: done" );
    }

    // Called by Native_onFish callback when a fish sound is detected
    // IMPORTANT: This is called from Java's game thread - do NOT do heavy work here
    void on_fish ( double x, double y, double z ) {
        // Early bail if not ready
        if ( !m_enabled.load ( ) ) return;
        if ( !m_java ) return;

        // Try to log - but be careful, this is called from Java thread
        try {
            LOG_INFO ( "on_fish callback! Sound at: " << x << ", " << y << ", " << z );
        } catch ( ... ) {
            // Ignore logging errors from callback
        }

        // Check if the sound location is within tolerance of our cached bobber position
        double distance = 0.0;
        {
            std::lock_guard<std::mutex> lock ( m_bobber_mutex );
            
            // Make sure we have a valid cached bobber
            if ( !m_cached_bobber.valid ) {
                return;
            }

            // Calculate distance from sound to cached bobber position
            double dx = x - m_cached_bobber.pos_x;
            double dy = y - m_cached_bobber.pos_y;
            double dz = z - m_cached_bobber.pos_z;
            distance = std::sqrt ( dx * dx + dy * dy + dz * dz );

            // If sound is too far from our bobber, ignore it (not our fish)
            if ( distance > m_sound_tolerance.load ( ) ) {
                return;
            }
        }

        try {
            LOG_SUCCESS ( "  FISH DETECTED! Distance: " << distance << " - Setting pending reel" );
        } catch ( ... ) { }

        // Mark that we need to reel - do the actual click from the main loop
        m_pending_reel.store ( true );
        m_last_catch_time = std::chrono::steady_clock::now ( );

        // Invalidate bobber cache since we're about to reel in
        {
            std::lock_guard<std::mutex> lock ( m_bobber_mutex );
            m_cached_bobber.valid = false;
        }
    }

    // Call this from main loop to process pending reel/cast actions
    void process_pending_actions ( ) {
        if ( !m_java ) return;
        if ( !m_enabled.load ( ) ) return;

        // Check if we have a pending reel action
        if ( m_pending_reel.exchange ( false ) ) {
            LOG_INFO ( "Processing pending reel action..." );
            reel_in ( );
            
            // Set pending recast after delay
            if ( m_auto_recast.load ( ) ) {
                m_recast_time = std::chrono::steady_clock::now ( ) + 
                    std::chrono::milliseconds ( m_recast_delay_ms.load ( ) );
                m_pending_recast.store ( true );
            }
        }

        // Check if we have a pending recast
        if ( m_pending_recast.load ( ) ) {
            auto now = std::chrono::steady_clock::now ( );
            if ( now >= m_recast_time ) {
                LOG_INFO ( "Processing pending recast..." );
                m_pending_recast.store ( false );
                cast ( );
            }
        }
    }

    // Right-click to reel in the fish or cast the rod
    void reel_in ( ) {
        if ( !m_java ) return;

        c_minecraft minecraft ( m_java );
        c_entity player = minecraft.get_player ( );
        c_world world = minecraft.get_world ( );
        c_player_controller_mp player_controller = minecraft.get_player_controller ( );

        if ( !player.get ( ) || !world.get ( ) || !player_controller.get ( ) ) return;

        // Clear right-click delay
        minecraft.set_right_click_delay ( 0 );

        // Get held item (fishing rod)
        jobject held_item = player.get_held_item ( ).get ( );
        if ( !held_item ) return;

        // Send right-click to reel in
        player_controller.use_item ( player.get ( ), world.get ( ), held_item );
    }

    // Cast the fishing rod (same as reel_in - right-click toggles)
    void cast ( ) {
        reel_in ( );
    }

    // Check if the player is holding a fishing rod
    [[nodiscard]] bool is_holding_fishing_rod ( ) const {
        if ( !m_java ) return false;

        c_minecraft minecraft ( m_java );
        c_entity player = minecraft.get_player ( );
        if ( !player.get ( ) ) return false;

        c_itemstack held_item = player.get_held_item ( );
        if ( !held_item.get ( ) ) return false;

        // Check if item is a fishing rod (item ID 346 in vanilla MC)
        // You may need to adjust this for modded clients
        c_item item = held_item.get_item ( );
        if ( !item.get ( ) ) return false;

        int item_id = item.get_id ( );
        return item_id == 346;  // Fishing rod ID
    }

    // Get cached bobber info (thread-safe)
    [[nodiscard]] bobber_info get_cached_bobber ( ) const {
        std::lock_guard<std::mutex> lock ( m_bobber_mutex );
        return m_cached_bobber;
    }

    // Check if we have a valid bobber cached
    [[nodiscard]] bool has_bobber ( ) const {
        std::lock_guard<std::mutex> lock ( m_bobber_mutex );
        return m_cached_bobber.valid;
    }

    // Settings
    void set_enabled ( bool enabled ) {
        LOG_INFO ( "set_enabled called with: " << ( enabled ? "true" : "false" ) );
        m_enabled.store ( enabled );
        
        // Update the Java-side flag via NativeBridge - DISABLED for now, may crash
        // if ( m_java ) {
        //     c_native_bridge bridge ( m_java );
        //     bridge.set_autofishing ( enabled );
        // }
        LOG_INFO ( "set_enabled completed" );
    }

    [[nodiscard]] bool is_enabled ( ) const { return m_enabled.load ( ); }

    void set_auto_recast ( bool enabled ) { m_auto_recast.store ( enabled ); }
    [[nodiscard]] bool get_auto_recast ( ) const { return m_auto_recast.load ( ); }

    void set_recast_delay ( int delay_ms ) { m_recast_delay_ms.store ( delay_ms ); }
    [[nodiscard]] int get_recast_delay ( ) const { return m_recast_delay_ms.load ( ); }

    void set_reel_delay ( int delay_ms ) { m_reel_delay_ms.store ( delay_ms ); }
    [[nodiscard]] int get_reel_delay ( ) const { return m_reel_delay_ms.load ( ); }

    void set_max_bobber_distance ( double distance ) { m_max_bobber_distance.store ( distance ); }
    [[nodiscard]] double get_max_bobber_distance ( ) const { return m_max_bobber_distance.load ( ); }

    void set_sound_tolerance ( double tolerance ) { m_sound_tolerance.store ( tolerance ); }
    [[nodiscard]] double get_sound_tolerance ( ) const { return m_sound_tolerance.load ( ); }
};
