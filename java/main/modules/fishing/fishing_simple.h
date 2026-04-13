#pragma once
#include "../../../java/java.h"
#include "../../../java/internal/minecraft.h"
#include "../../../java/internal/native_bridge.h"
#include "../../../utils/file_logging.h"
#include <chrono>
#include <atomic>
#include <cmath>
#include <thread>
#include <Windows.h>

// Window enumeration callback data
struct window_enum_data {
    DWORD target_pid;
    HWND result;
};

// Callback for EnumWindows to find game window
inline BOOL CALLBACK enum_windows_callback ( HWND hwnd, LPARAM lparam ) {
    auto * data = reinterpret_cast<window_enum_data *> ( lparam );
    
    DWORD window_pid;
    GetWindowThreadProcessId ( hwnd, &window_pid );
    
    if ( window_pid != data->target_pid ) return TRUE;
    
    // Check if visible and has a title
    if ( !IsWindowVisible ( hwnd ) ) return TRUE;
    
    char title [ 256 ];
    GetWindowTextA ( hwnd, title, sizeof ( title ) );
    
    // Look for cosmic or minecraft in title (case-insensitive)
    std::string title_lower = title;
    for ( auto & c : title_lower ) c = static_cast<char> ( tolower ( c ) );
    
    if ( title_lower.find ( "cosmic" ) != std::string::npos || 
         title_lower.find ( "minecraft" ) != std::string::npos ) {
        data->result = hwnd;
        return FALSE;  // Stop enumeration
    }
    
    return TRUE;
}

// Fishing module using bobber position detection
// Detects when bobber gets yanked down (Y position drops suddenly)
class c_fishing {
private:
    c_java * m_java;
    std::atomic<bool> m_enabled { false };
    std::atomic<bool> m_auto_recast { true };
    std::atomic<int> m_recast_delay_ms { 500 };  // ms delay before recasting
    
    // Bobber tracking
    int m_bobber_entity_id { -1 };
    double m_last_motion_y { 0.0 };
    double m_last_pos_y { 0.0 };
    double m_stable_pos_y { 0.0 };  // Y position when bobber stabilized
    bool m_bobber_stable { false };
    std::chrono::steady_clock::time_point m_bobber_spawn_time;
    int m_debug_counter { 0 };
    
    // Pending actions (processed from main loop)
    std::atomic<bool> m_pending_reel { false };
    std::atomic<bool> m_pending_recast { false };
    std::chrono::steady_clock::time_point m_recast_time;
    std::chrono::steady_clock::time_point m_last_catch_time;
    
    // Stats
    std::atomic<int> m_fish_caught { 0 };
    
    // Window handle cache
    HWND m_game_window { nullptr };
    
    // Position drop threshold for fish detection (bobber drops ~0.5 blocks when fish bites)
    static constexpr double DROP_THRESHOLD = 0.2;  // Bobber drops more than 0.2 blocks = fish
    
    // Find and cache the game window
    HWND find_game_window ( ) {
        // Check if cached window is still valid
        if ( m_game_window && IsWindow ( m_game_window ) ) {
            return m_game_window;
        }
        
        // Find window in our process
        window_enum_data data { };
        data.target_pid = GetCurrentProcessId ( );
        data.result = nullptr;
        
        EnumWindows ( enum_windows_callback, reinterpret_cast<LPARAM> ( &data ) );
        
        if ( data.result ) {
            m_game_window = data.result;
            LOG_INFO ( "Found game window: " << m_game_window );
        }
        
        return m_game_window;
    }

public:
    explicit c_fishing ( c_java * java ) : m_java ( java ) { }

    // Scan for bobber and check its position
    void update ( ) {
        if ( !m_enabled.load ( ) ) return;
        if ( !m_java ) return;

        c_minecraft minecraft ( m_java );
        c_entity player = minecraft.get_player ( );
        c_world world = minecraft.get_world ( );

        if ( !player.get ( ) || !world.get ( ) ) return;

        // Check if EntityFishHook class is cached
        jclass fish_hook_class = m_java->get_class ( "EntityFishHook" );
        if ( !fish_hook_class ) return;

        JNIEnv * env = m_java->get_env ( );
        
        // Get player position for distance check
        double player_x = player.get_pos_x ( );
        double player_y = player.get_pos_y ( );
        double player_z = player.get_pos_z ( );

        // Get all entities
        std::vector<c_entity> entities = world.get_entities ( );
        
        // Push local frame for safety
        if ( env->PushLocalFrame ( 128 ) < 0 ) return;
        
        bool found_bobber = false;
        
        for ( auto & entity : entities ) {
            if ( env->ExceptionCheck ( ) ) {
                env->ExceptionClear ( );
                continue;
            }
            
            if ( !entity.get ( ) ) continue;
            
            // Check if fishing bobber
            if ( !env->IsInstanceOf ( entity.get ( ), fish_hook_class ) ) continue;
            
            // Check distance (should be within 64 blocks)
            double bx = entity.get_pos_x ( );
            double by = entity.get_pos_y ( );
            double bz = entity.get_pos_z ( );
            double dx = bx - player_x;
            double dy = by - player_y;
            double dz = bz - player_z;
            double dist = std::sqrt ( dx * dx + dy * dy + dz * dz );
            
            if ( dist > 64.0 ) continue;
            
            found_bobber = true;
            
            // Get current motionY and posY
            double motion_y = entity.get_motion_y ( );
            
            // Only check if bobber has been out for at least 2 seconds (to avoid cast motion)
            auto now = std::chrono::steady_clock::now ( );
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds> ( now - m_bobber_spawn_time ).count ( );
            
            if ( m_bobber_entity_id == -1 ) {
                // New bobber detected
                m_bobber_entity_id = 1;
                m_bobber_spawn_time = now;
                m_last_motion_y = motion_y;
                m_last_pos_y = by;
                m_stable_pos_y = by;
                m_bobber_stable = false;
                m_debug_counter = 0;
                LOG_SUCCESS ( "Bobber detected at: " << bx << ", " << by << ", " << bz );
            } else if ( elapsed > 2500 ) {
                // Bobber has been out long enough
                
                // Debug: log values every 2 seconds
                m_debug_counter++;
                if ( m_debug_counter % 40 == 0 ) {  // ~every 2 seconds at 50ms tick
                    LOG_INFO ( "Bobber: Y=" << by << " motionY=" << motion_y << " stable=" << m_stable_pos_y );
                }
                
                // Mark stable position once bobber settles (small motion)
                if ( !m_bobber_stable && std::abs ( motion_y ) < 0.05 ) {
                    m_bobber_stable = true;
                    m_stable_pos_y = by;
                    LOG_INFO ( "Bobber stabilized at Y=" << by );
                }
                
                // Check for fish bite: bobber drops significantly from stable position
                if ( m_bobber_stable ) {
                    double drop = m_stable_pos_y - by;
                    
                    if ( drop > DROP_THRESHOLD ) {
                        LOG_SUCCESS ( "FISH BITE! Y dropped " << drop << " blocks (from " << m_stable_pos_y << " to " << by << ")" );
                        m_pending_reel.store ( true );
                        m_fish_caught++;
                        m_bobber_entity_id = -1;  // Reset bobber tracking
                    }
                }
            }
            
            m_last_motion_y = motion_y;
            m_last_pos_y = by;
            break;  // Found our bobber
        }
        
        // If no bobber found, reset
        if ( !found_bobber && m_bobber_entity_id != -1 ) {
            LOG_INFO ( "Bobber gone" );
            m_bobber_entity_id = -1;
        }
        
        env->PopLocalFrame ( nullptr );
    }

    // Called by Native_onFish callback (backup detection via sound)
    void on_fish ( double x, double y, double z ) {
        if ( !m_enabled.load ( ) ) return;
        
        LOG_SUCCESS ( "FISH (sound)! Splash at: " << x << ", " << y << ", " << z );
        m_pending_reel.store ( true );
        m_fish_caught++;
        m_bobber_entity_id = -1;
    }

    // Call this from main loop to process pending reel/cast actions
    void process_pending_actions ( ) {
        if ( !m_java ) return;
        if ( !m_enabled.load ( ) ) return;

        // Check if we have a pending reel action
        if ( m_pending_reel.exchange ( false ) ) {
            LOG_INFO ( "Reeling in..." );
            right_click ( );
            
            // Schedule recast after delay
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
                LOG_INFO ( "Recasting..." );
                m_pending_recast.store ( false );
                right_click ( );
            }
        }
    }

    // Perform a right-click action using PostMessage to game window
    void right_click ( ) {
        HWND hwnd = find_game_window ( );
        
        if ( hwnd ) {
            // Get client area size
            RECT rect;
            GetClientRect ( hwnd, &rect );
            int center_x = ( rect.right - rect.left ) / 2;
            int center_y = ( rect.bottom - rect.top ) / 2;
            
            const LPARAM l_param = MAKELPARAM ( center_x, center_y );
            
            // Send right-click via window message
            PostMessage ( hwnd, WM_RBUTTONDOWN, MK_RBUTTON, l_param );
            std::this_thread::sleep_for ( std::chrono::milliseconds ( 50 ) );
            PostMessage ( hwnd, WM_RBUTTONUP, 0, l_param );
            
            LOG_INFO ( "right_click: PostMessage to " << hwnd << " at (" << center_x << ", " << center_y << ")" );
        }
        else {
            // Fallback to SendInput
            INPUT input = { 0 };
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            SendInput ( 1, &input, sizeof ( INPUT ) );
            std::this_thread::sleep_for ( std::chrono::milliseconds ( 50 ) );
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            SendInput ( 1, &input, sizeof ( INPUT ) );
            
            LOG_WARNING ( "right_click: fallback to SendInput (no window)" );
        }
    }

    void set_enabled ( bool enabled ) {
        LOG_INFO ( "set_enabled called with: " << ( enabled ? "true" : "false" ) );
        m_enabled.store ( enabled );
        m_bobber_entity_id = -1;  // Reset tracking
        LOG_INFO ( "set_enabled completed" );
    }

    [[nodiscard]] bool is_enabled ( ) const {
        return m_enabled.load ( );
    }

    [[nodiscard]] int get_fish_caught ( ) const {
        return m_fish_caught.load ( );
    }

    void set_auto_recast ( bool enabled ) {
        m_auto_recast.store ( enabled );
    }

    void set_recast_delay ( int ms ) {
        m_recast_delay_ms.store ( ms );
    }

    // Compatibility stubs
    void update_bobber ( ) { update ( ); }
    bool has_bobber ( ) const { return m_bobber_entity_id != -1; }
    struct { double pos_x = 0; double pos_y = 0; double pos_z = 0; } get_cached_bobber ( ) const { return {}; }
};
