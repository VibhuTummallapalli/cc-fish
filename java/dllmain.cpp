#include <Windows.h>
#include <iostream>
#include <thread>
#include <exception>

#include "utils/logging.h"
#include "java/java.h"
#include "main/hack.h"

// Global Java instance
c_java g_java;
bool g_initialized = false;

// Update console title with current status
void update_status_title ( bool enabled, bool has_bobber ) {
    try {
        std::string title = "Fish - Auto-Fisher | Status: ";
        if ( enabled ) {
            title += "ENABLED";
            if ( has_bobber ) {
                title += " | Bobber: TRACKING";
            } else {
                title += " | Bobber: NOT FOUND";
            }
        } else {
            title += "DISABLED";
        }
        title += " | INSERT=Toggle, END=Eject";
        SetConsoleTitleA ( title.c_str ( ) );
    } catch ( ... ) {
        // Ignore errors
    }
}

// Allocate console for debug output
void allocate_console ( ) {
    AllocConsole ( );
    
    FILE * f_dummy;
    freopen_s ( &f_dummy, "CONIN$", "r", stdin );
    freopen_s ( &f_dummy, "CONOUT$", "w", stderr );
    freopen_s ( &f_dummy, "CONOUT$", "w", stdout );

    // Enable ANSI color codes on Windows 10+
    HANDLE h_out = GetStdHandle ( STD_OUTPUT_HANDLE );
    DWORD dw_mode = 0;
    GetConsoleMode ( h_out, &dw_mode );
    dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode ( h_out, dw_mode );

    SetConsoleTitleA ( "Fish - Minecraft AutoFisher [Loading...]" );

    LOG_INFO ( "Console allocated" );
}

// Safe main loop (separate from init)
void main_loop ( HMODULE h_module ) {
    LOG_INFO ( "" );
    LOG_INFO ( "Controls:" );
    LOG_INFO ( "  INSERT  - Toggle auto-fishing" );
    LOG_INFO ( "  END     - Eject DLL" );
    LOG_INFO ( "  HOME    - Update bobber position manually" );
    LOG_INFO ( "" );

    // Print initial status
    std::cout << std::endl;
    std::cout << oof::fg_color ( { 50, 255, 50 } ) << "  ========================================" << oof::reset_formatting ( ) << std::endl;
    std::cout << oof::fg_color ( { 50, 255, 50 } ) << "  ||  AUTO-FISHING: " << oof::fg_color ( { 100, 255, 100 } ) << "ENABLED" << oof::fg_color ( { 50, 255, 50 } ) << "            ||" << oof::reset_formatting ( ) << std::endl;
    std::cout << oof::fg_color ( { 50, 255, 50 } ) << "  ========================================" << oof::reset_formatting ( ) << std::endl;
    std::cout << std::endl;

    // Main loop
    bool running = true;
    auto last_status_print = std::chrono::steady_clock::now ( );
    bool last_had_bobber = false;

    while ( running ) {
        try {
            if ( !g_hack ) {
                std::this_thread::sleep_for ( std::chrono::milliseconds ( 100 ) );
                continue;
            }

            bool is_enabled = g_hack->module_handler.get_fishing ( ).is_enabled ( );
            bool has_bobber = g_hack->module_handler.get_fishing ( ).has_bobber ( );

            // Update console title with status
            update_status_title ( is_enabled, has_bobber );

            // Toggle auto-fishing with INSERT key
            if ( GetAsyncKeyState ( VK_INSERT ) & 1 ) {
                bool current = g_hack->module_handler.get_fishing ( ).is_enabled ( );
                g_hack->module_handler.get_fishing ( ).set_enabled ( !current );
                
                std::cout << std::endl;
                if ( !current ) {
                    std::cout << oof::fg_color ( { 50, 255, 50 } ) << "  ========================================" << oof::reset_formatting ( ) << std::endl;
                    std::cout << oof::fg_color ( { 50, 255, 50 } ) << "  ||  AUTO-FISHING: " << oof::fg_color ( { 100, 255, 100 } ) << "ENABLED" << oof::fg_color ( { 50, 255, 50 } ) << "            ||" << oof::reset_formatting ( ) << std::endl;
                    std::cout << oof::fg_color ( { 50, 255, 50 } ) << "  ========================================" << oof::reset_formatting ( ) << std::endl;
                } else {
                    std::cout << oof::fg_color ( { 255, 100, 100 } ) << "  ========================================" << oof::reset_formatting ( ) << std::endl;
                    std::cout << oof::fg_color ( { 255, 100, 100 } ) << "  ||  AUTO-FISHING: " << oof::fg_color ( { 255, 50, 50 } ) << "DISABLED" << oof::fg_color ( { 255, 100, 100 } ) << "           ||" << oof::reset_formatting ( ) << std::endl;
                    std::cout << oof::fg_color ( { 255, 100, 100 } ) << "  ========================================" << oof::reset_formatting ( ) << std::endl;
                }
                std::cout << std::endl;
            }

            // Manual bobber update with HOME key
            if ( GetAsyncKeyState ( VK_HOME ) & 1 ) {
                g_hack->module_handler.get_fishing ( ).update_bobber ( );
                
                if ( g_hack->module_handler.get_fishing ( ).has_bobber ( ) ) {
                    auto bobber = g_hack->module_handler.get_fishing ( ).get_cached_bobber ( );
                    LOG_INFO ( "Bobber found at: " << bobber.pos_x << ", " << bobber.pos_y << ", " << bobber.pos_z );
                } else {
                    LOG_WARNING ( "No bobber found in world" );
                }
            }

            // Eject with END key
            if ( GetAsyncKeyState ( VK_END ) & 1 ) {
                LOG_WARNING ( "Ejecting..." );
                running = false;
            }

            // Periodically update bobber position (every 100ms)
            static auto last_update = std::chrono::steady_clock::now ( );
            auto now = std::chrono::steady_clock::now ( );
            if ( std::chrono::duration_cast<std::chrono::milliseconds> ( now - last_update ).count ( ) > 100 ) {
                if ( g_hack->module_handler.get_fishing ( ).is_enabled ( ) ) {
                    g_hack->module_handler.get_fishing ( ).update_bobber ( );
                }
                last_update = now;
            }

            // Print bobber status when it changes
            if ( is_enabled && has_bobber != last_had_bobber ) {
                if ( has_bobber ) {
                    LOG_SUCCESS ( "Bobber detected - Watching for fish!" );
                } else {
                    LOG_INFO ( "Bobber lost - Cast your rod!" );
                }
                last_had_bobber = has_bobber;
            }

            // Periodic status reminder (every 30 seconds)
            if ( std::chrono::duration_cast<std::chrono::seconds> ( now - last_status_print ).count ( ) >= 30 ) {
                if ( is_enabled ) {
                    if ( has_bobber ) {
                        std::cout << oof::fg_color ( { 100, 200, 100 } ) << "[Status] " << oof::reset_formatting ( ) << "Auto-fishing ENABLED - Bobber TRACKED" << std::endl;
                    } else {
                        std::cout << oof::fg_color ( { 200, 200, 100 } ) << "[Status] " << oof::reset_formatting ( ) << "Auto-fishing ENABLED - Waiting for bobber..." << std::endl;
                    }
                }
                last_status_print = now;
            }
        } catch ( const std::exception & e ) {
            LOG_ERROR ( "Exception in main loop: " << e.what ( ) );
        } catch ( ... ) {
            LOG_ERROR ( "Unknown exception in main loop!" );
        }

        std::this_thread::sleep_for ( std::chrono::milliseconds ( 10 ) );
    }

    // Cleanup
    LOG_INFO ( "Cleaning up..." );
    
    if ( g_hack ) {
        g_hack->module_handler.get_fishing ( ).set_enabled ( false );
        delete g_hack;
        g_hack = nullptr;
    }

    g_java.shutdown ( );

    // Free console
    FreeConsole ( );

    // Unload the DLL
    FreeLibraryAndExitThread ( h_module, 0 );
}

// Main initialization thread
void main_thread ( HMODULE h_module ) {
    // Give the game a moment to stabilize
    Sleep ( 2000 );

    try {
        allocate_console ( );

        LOG_INFO ( "=====================================" );
        LOG_INFO ( "   Fish - Minecraft Auto-Fisher" );
        LOG_INFO ( "=====================================" );
        LOG_INFO ( "" );
        LOG_INFO ( "DLL loaded successfully!" );
        LOG_INFO ( "Press any key if console closes immediately..." );
        LOG_INFO ( "" );

        // Initialize Java/JNI bridge
        LOG_INFO ( "Step 1: Looking for JVM module..." );
        std::cout.flush ( );
        
        if ( !g_java.initialize ( ) ) {
            LOG_ERROR ( "Failed to initialize Java bridge!" );
            LOG_ERROR ( "Make sure you're injecting into Minecraft with a JVM running." );
            LOG_INFO ( "" );
            LOG_INFO ( "Press ENTER to close..." );
            std::cin.get ( );
            FreeConsole ( );
            FreeLibraryAndExitThread ( h_module, 0 );
            return;
        }

        LOG_SUCCESS ( "Java bridge initialized!" );

        // Create the hack instance
        LOG_INFO ( "Step 2: Creating hack instance..." );
        std::cout.flush ( );
        
        g_hack = new c_hack ( &g_java );
        g_hack->initialize ( );

        LOG_SUCCESS ( "Hack instance created" );
        g_initialized = true;

        // Enable auto-fishing by default
        g_hack->module_handler.get_fishing ( ).set_enabled ( true );
        LOG_SUCCESS ( "Auto-fishing enabled!" );

        // Enter main loop
        main_loop ( h_module );

    } catch ( const std::exception & e ) {
        LOG_ERROR ( "CRITICAL EXCEPTION: " << e.what ( ) );
        LOG_INFO ( "" );
        LOG_INFO ( "Press any key to close..." );
        std::cin.get ( );
        FreeConsole ( );
        FreeLibraryAndExitThread ( h_module, 0 );
    } catch ( ... ) {
        LOG_ERROR ( "UNKNOWN CRITICAL EXCEPTION!" );
        LOG_INFO ( "" );
        LOG_INFO ( "Press any key to close..." );
        std::cin.get ( );
        FreeConsole ( );
        FreeLibraryAndExitThread ( h_module, 0 );
    }
}

BOOL APIENTRY DllMain ( HMODULE h_module, DWORD ul_reason_for_call, LPVOID lp_reserved ) {
    switch ( ul_reason_for_call ) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls ( h_module );
            // Create main thread
            if ( HANDLE h_thread = CreateThread ( nullptr, 0, 
                reinterpret_cast<LPTHREAD_START_ROUTINE> ( main_thread ), 
                h_module, 0, nullptr ) ) {
                CloseHandle ( h_thread );
            }
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
