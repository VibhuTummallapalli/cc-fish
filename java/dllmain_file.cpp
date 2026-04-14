#include <Windows.h>
#include <ShlObj.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <exception>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

#pragma comment(lib, "shell32.lib")

#include "java/java.h"
#include "main/hack.h"

// Global Java instance
c_java g_java;
bool g_initialized = false;
std::ofstream g_log_file;

// File-based logging
void fish_log ( const std::string & level, const std::string & msg ) {
    if ( g_log_file.is_open ( ) ) {
        auto now = std::chrono::system_clock::now ( );
        auto time = std::chrono::system_clock::to_time_t ( now );
        std::tm tm_buf;
        localtime_s ( &tm_buf, &time );
        
        g_log_file << "[" << std::put_time ( &tm_buf, "%H:%M:%S" ) << "] "
                   << "[" << level << "] " << msg << std::endl;
        g_log_file.flush ( );
    }
}

#define FISH_LOG(msg) { std::ostringstream ss; ss << msg; fish_log("INFO", ss.str()); }
#define FISH_OK(msg) { std::ostringstream ss; ss << msg; fish_log("OK", ss.str()); }
#define FISH_ERR(msg) { std::ostringstream ss; ss << msg; fish_log("ERROR", ss.str()); }
#define FISH_WARN(msg) { std::ostringstream ss; ss << msg; fish_log("WARN", ss.str()); }

void open_log_file ( ) {
    char path [ MAX_PATH ];
    std::string log_path;
    
    if ( SUCCEEDED ( SHGetFolderPathA ( NULL, CSIDL_DESKTOP, NULL, 0, path ) ) ) {
        log_path = std::string ( path ) + "\\fish_log.txt";
    } else {
        log_path = "C:\\fish_log.txt";
    }
    
    g_log_file.open ( log_path, std::ios::out | std::ios::trunc );
    
    if ( g_log_file.is_open ( ) ) {
        FISH_LOG ( "=====================================");
        FISH_LOG ( "   Fish - Minecraft Auto-Fisher" );
        FISH_LOG ( "=====================================" );
        FISH_LOG ( "Log file: " << log_path );
    }
}

// Safe main loop
void main_loop ( HMODULE h_module ) {
    FISH_LOG ( "" );
    FISH_LOG ( "Controls:" );
    FISH_LOG ( "  INSERT  - Toggle auto-fishing" );
    FISH_LOG ( "  END     - Eject DLL" );
    FISH_LOG ( "  HOME    - Show fish count" );
    FISH_LOG ( "" );
    FISH_OK ( "Auto-fishing ENABLED" );

    // Show notification
    MessageBoxA ( NULL, 
        "Fish Auto-Fisher loaded!\n\n"
        "Controls:\n"
        "  INSERT = Toggle fishing\n"
        "  HOME = Show fish count\n"
        "  END = Unload\n\n"
        "Cast your line then enable!\n"
        "Check fish_log.txt on Desktop for status.",
        "Fish", MB_OK | MB_ICONINFORMATION );

    FISH_LOG ( "Starting main loop..." );

    bool running = true;
    auto last_log_time = std::chrono::steady_clock::now ( );
    int loop_count = 0;

    while ( running ) {
        loop_count++;
        
        // Log every 100 iterations initially to track where crash happens
        if ( loop_count <= 10 || loop_count % 100 == 0 ) {
            FISH_LOG ( "Loop iteration: " << loop_count );
        }

        // Safety check
        if ( !g_hack || !g_initialized ) {
            FISH_ERR ( "g_hack is null or not initialized!" );
            Sleep ( 1000 );
            continue;
        }

        // Toggle with INSERT
        if ( GetAsyncKeyState ( VK_INSERT ) & 1 ) {
            FISH_LOG ( "INSERT key pressed" );
            try {
                bool current = g_hack->module_handler.get_fishing ( ).is_enabled ( );
                g_hack->module_handler.get_fishing ( ).set_enabled ( !current );
                
                if ( !current ) {
                    FISH_OK ( "Auto-fishing ENABLED" );
                } else {
                    FISH_WARN ( "Auto-fishing DISABLED" );
                }
            } catch ( ... ) {
                FISH_ERR ( "Exception toggling fishing" );
            }
        }

        // Eject with END
        if ( GetAsyncKeyState ( VK_END ) & 1 ) {
            FISH_WARN ( "END pressed - Ejecting..." );
            running = false;
            continue;
        }

        // HOME key - show fish count
        if ( GetAsyncKeyState ( VK_HOME ) & 1 ) {
            int count = g_hack->module_handler.get_fishing ( ).get_fish_caught ( );
            FISH_OK ( "Fish caught so far: " << count );
        }

        // Periodic status (every 60 seconds)
        auto now = std::chrono::steady_clock::now ( );
        if ( std::chrono::duration_cast<std::chrono::seconds> ( now - last_log_time ).count ( ) >= 60 ) {
            int count = g_hack->module_handler.get_fishing ( ).get_fish_caught ( );
            FISH_LOG ( "Still running... Fish caught: " << count );
            last_log_time = now;
        }

        // Update bobber tracking (velocity-based fish detection)
        try {
            g_hack->module_handler.get_fishing ( ).update ( );
        } catch ( ... ) {
            // Silently ignore update errors
        }

        // Process pending actions from fishing callbacks (reel in, recast)
        try {
            g_hack->module_handler.get_fishing ( ).process_pending_actions ( );
        } catch ( ... ) {
            FISH_ERR ( "Exception in process_pending_actions" );
        }

        Sleep ( 20 );  // Faster polling to catch brief fish bites
    }

    // Cleanup
    FISH_LOG ( "Cleaning up..." );
    
    if ( g_hack ) {
        g_hack->module_handler.get_fishing ( ).set_enabled ( false );
        delete g_hack;
        g_hack = nullptr;
    }

    g_java.shutdown ( );

    FISH_LOG ( "Goodbye!" );
    g_log_file.close ( );

    FreeLibraryAndExitThread ( h_module, 0 );
}

// Main initialization thread
void main_thread ( HMODULE h_module ) {
    Sleep ( 2000 );  // Let game stabilize

    // Open log FIRST before anything else
    open_log_file ( );
    FISH_LOG ( "DLL thread started" );

    try {
        FISH_LOG ( "" );
        FISH_LOG ( "Step 1: Looking for JVM..." );
        
        if ( !g_java.initialize ( ) ) {
            FISH_ERR ( "Failed to initialize Java bridge!" );
            MessageBoxA ( NULL, "Failed to initialize!\n\nCheck fish_log.txt on Desktop.", "Fish Error", MB_OK | MB_ICONERROR );
            g_log_file.close ( );
            FreeLibraryAndExitThread ( h_module, 0 );
            return;
        }

        FISH_OK ( "Java bridge ready!" );

        FISH_LOG ( "Step 2: Creating hack instance..." );
        g_hack = new c_hack ( &g_java );
        
        if ( !g_hack ) {
            FISH_ERR ( "Failed to allocate hack instance!" );
            g_log_file.close ( );
            FreeLibraryAndExitThread ( h_module, 0 );
            return;
        }
        
        FISH_LOG ( "Step 3: Initializing hack..." );
        g_hack->initialize ( );

        FISH_OK ( "Hack instance created" );
        g_initialized = true;

        FISH_LOG ( "Step 4: Enabling fishing module..." );
        g_hack->module_handler.get_fishing ( ).set_enabled ( true );
        FISH_OK ( "Auto-fishing enabled!" );

        FISH_LOG ( "Step 5: Entering main loop..." );
        main_loop ( h_module );

    } catch ( const std::exception & e ) {
        FISH_ERR ( "CRITICAL: " << e.what ( ) );
        MessageBoxA ( NULL, "Critical error!\n\nCheck fish_log.txt on Desktop.", "Fish Error", MB_OK | MB_ICONERROR );
        g_log_file.close ( );
        FreeLibraryAndExitThread ( h_module, 0 );
    } catch ( ... ) {
        FISH_ERR ( "UNKNOWN CRITICAL EXCEPTION!" );
        MessageBoxA ( NULL, "Unknown critical error!", "Fish Error", MB_OK | MB_ICONERROR );
        g_log_file.close ( );
        FreeLibraryAndExitThread ( h_module, 0 );
    }
}

BOOL APIENTRY DllMain ( HMODULE h_module, DWORD ul_reason_for_call, LPVOID lp_reserved ) {
    switch ( ul_reason_for_call ) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls ( h_module );
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
