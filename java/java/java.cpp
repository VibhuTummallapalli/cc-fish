#include "java.h"
#include "../utils/file_logging.h"
#include "native_bridge.h"
#include "mappings/native_bridge_mappings.h"
#include "mappings/cosmic_client_mappings.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ranges>
#include <cstring>
#include <Windows.h>

// redefined classes
#include "redefined_classes/render_living_entity.h"
#include "redefined_classes/entity_renderer.h"
#include "redefined_classes/net_handler_play_client.h"
#include "redefined_classes/entity_player.h"
#include "redefined_classes/abstract_client_player.h"
#include "redefined_classes/world.h"

#include "../main/hack.h"
#include "redefined_classes/cosmic_proxy.h"
#include "redefined_classes/minecraft.h"

// Function pointer type for JNI_GetCreatedJavaVMs
typedef jint ( JNICALL * fnGetCreatedJavaVMs )( JavaVM **, jsize, jsize * );
static fnGetCreatedJavaVMs g_GetCreatedJavaVMs = nullptr;

// Find JVM module in current process
static HMODULE find_jvm_module ( ) {
    // Try common JVM module names
    const char * jvm_names [ ] = { 
        "jvm.dll", 
        "libjvm.dll",
        "server\\jvm.dll",
        "client\\jvm.dll"
    };
    
    for ( const char * name : jvm_names ) {
        HMODULE h = GetModuleHandleA ( name );
        if ( h ) {
            LOG_INFO ( "Found JVM module: " << name );
            return h;
        }
    }

    LOG_ERROR ( "Could not find jvm.dll via GetModuleHandle" );
    return nullptr;
}

// Native method implementations
static void JNICALL Native_onRenderEntityStart ( JNIEnv * env, jclass cls, jlong uuidMost, jlong uuidLeast ) {
    if ( g_hack ) g_hack->chams.on_render_entity_start ( static_cast < long long > ( uuidMost ), static_cast < long long > ( uuidLeast ) );
}

static void JNICALL Native_onRenderEntityEnd ( JNIEnv * env, jclass cls, jlong uuidMost, jlong uuidLeast ) {
    if ( g_hack ) g_hack->chams.on_render_entity_end ( static_cast < long long > ( uuidMost ), static_cast < long long > ( uuidLeast ) );
}

static void JNICALL Native_onFish ( JNIEnv * env, jclass cls, jdouble x, jdouble y, jdouble z ) {
    if ( g_hack ) g_hack->module_handler.get_fishing ( ).on_fish ( x, y, z );
}

c_java::c_java ( )
    : m_env ( nullptr )
    , m_jvmti ( nullptr )
    , m_jvm ( nullptr )
    , m_main_thread_class_loader ( nullptr )
    , m_minecraft ( nullptr )
    , m_cosmic_client ( nullptr )
    , m_native_bridge_class ( nullptr )
    , m_set_injected_method ( nullptr )
{
}

bool c_java::initialize ( ) {
    LOG_INFO ( "  [1/5] Attaching to JVM..." );
    
    if ( attach_to_jvm ( ) ) {
        LOG_SUCCESS ( "  Attached to JVM" );
    }
    else {
        LOG_ERROR ( "  Failed to attach to JVM" );
        return false;
    }

    LOG_INFO ( "  [2/5] Setting up class loader..." );
    
    if ( !setup_class_loader ( ) ) {
        LOG_ERROR ( "  Failed to setup class loader" );
        return false;
    }
    LOG_SUCCESS ( "  Class loader ready" );

    LOG_INFO ( "  [3/5] Setting up JVMTI..." );
    
    if ( !setup_jvmti ( ) ) {
        LOG_WARNING ( "  JVMTI setup failed (DisableAttachMechanism may be enabled)" );
        LOG_WARNING ( "  Native bridge will not be available - continuing without it..." );
        // Don't return false - we can still work without JVMTI for basic fishing
    } else {
        LOG_SUCCESS ( "  JVMTI ready" );
    }

    LOG_INFO ( "  [4/5] Loading mappings..." );

    // all mappings
    std::vector<mappings::mapping_set> mapping_sets;

    // cosmic client mappings
    mappings::mapping_set cosmic_mappings;
    mappings::register_cosmic_client_mappings ( cosmic_mappings );
    mapping_sets.push_back ( cosmic_mappings );

    // todo: demonicclient, lunar, vanilla

    // Try each mapping set until one works
    for ( const auto & mappings : mapping_sets ) {
        LOG_INFO ( "  Testing mapping set: " << mappings.name.c_str ( ) );

        if ( verify_mappings ( mappings ) ) {
            LOG_SUCCESS ( "  Verified mapping set: " << oof::fg_color ( { 100, 150, 255 } ) << mappings.name.c_str ( ) );

            // set the active mappings
            m_active_mappings = mappings;

            // cache them
            LOG_INFO ( "  [5/5] Caching mappings..." );
            cache_all_mappings ( );

            // load minecraft instance
            LOG_INFO ( "  Getting Minecraft instance..." );
            m_minecraft = m_env->NewGlobalRef ( m_env->CallStaticObjectMethod ( get_class ( xorstr_ ( "Minecraft" ) ), get_method ( xorstr_ ( "Minecraft.getMinecraft" ) ) ) );

            if ( !m_minecraft ) {
                LOG_ERROR ( "  Failed to get Minecraft instance" );
                return false;
            }
            LOG_SUCCESS ( "  Got Minecraft instance" );

			// only load the cosmic client class if we are using cosmic client mappings, also only handle the bridge if cosmic client
            if ( m_jvmti ) {
                // JVMTI is available - set up native bridge and redefine classes
                if ( auto cosmic_client_class = get_class ( xorstr_ ( "CosmicClientHandler" ) ) ) {
                    m_cosmic_client = m_env->NewGlobalRef ( m_env->CallStaticObjectMethod ( cosmic_client_class, get_method ( xorstr_ ( "CosmicClientHandler.getInstance" ) ) ) );

                    if ( !define_native_bridge_class ( ) ) {
                        LOG_WARNING ( "Failed to define native bridge class - fishing callbacks won't work" );
                    } else {
                        mappings::register_native_bridge_mappings ( m_active_mappings );
                        m_classes [ "NativeBridge" ] = m_native_bridge_class;
                        cache_native_bridge_mappings ( );

                        ( void ) redefine_class_from_memory ( xorstr_ ( "cosmicclient/RS" ), entity_render_class, entity_render_class_size );
                        ( void ) redefine_class_from_memory ( xorstr_ ( "cosmicclient/aXU" ), render_entity_living_class, render_entity_living_class_size );
                        ( void ) redefine_class_from_memory ( xorstr_ ( "cosmicclient/OM" ), entity_player_class, entity_player_class_size );
                        ( void ) redefine_class_from_memory ( xorstr_ ( "cosmicclient/r6" ), net_handler_play_client_class, net_handler_play_client_class_size );
                        ( void ) redefine_class_from_memory ( xorstr_ ( "cosmicclient/O4" ), abstract_client_player_class, abstract_client_player_class_size );
                        ( void ) redefine_class_from_memory ( xorstr_ ( "cosmicclient/YU" ), world_class, world_class_size );
                        ( void ) redefine_class_from_memory ( xorstr_ ( "cosmicclient/wb" ), minecraft_class, minecraft_class_size );
                        //( void ) redefine_class_from_memory ( xorstr_ ( "cosmicclient/bu" ), cosmic_proxy_class, cosmic_proxy_class_size );

                        if ( !register_native_bridge_methods ( ) ) {
                            LOG_WARNING ( "Failed to register native methods - fish callbacks disabled" );
                        } else {
                            LOG_SUCCESS ( "  Native bridge ready" );
                        }
                    }
                }
            } else {
                LOG_WARNING ( "  JVMTI not available - running in limited mode (no fish callbacks)" );
            }

            return true;
        }

        LOG_ERROR ( "Mapping set " << oof::fg_color ( { 235, 40, 63 } ) << mappings.name.c_str ( ) << oof::reset_formatting ( ) << " failed, trying next..." );
    }

    LOG_ERROR ( "No valid mapping sets found!" );
    return false;
}

bool c_java::attach_to_jvm ( ) {
    // Dynamically load JNI_GetCreatedJavaVMs from jvm.dll
    HMODULE jvm_module = find_jvm_module ( );
    if ( !jvm_module ) {
        LOG_ERROR ( "Could not find jvm.dll in process!" );
        LOG_ERROR ( "Make sure Minecraft/Java is fully loaded before injecting." );
        return false;
    }

    g_GetCreatedJavaVMs = ( fnGetCreatedJavaVMs ) GetProcAddress ( jvm_module, "JNI_GetCreatedJavaVMs" );
    if ( !g_GetCreatedJavaVMs ) {
        LOG_ERROR ( "Could not find JNI_GetCreatedJavaVMs in jvm.dll" );
        return false;
    }

    LOG_INFO ( "Found JNI_GetCreatedJavaVMs function" );

    jsize count = 0;
    if ( g_GetCreatedJavaVMs ( &m_jvm, 1, &count ) != JNI_OK || count == 0 ) {
        LOG_ERROR ( "JNI_GetCreatedJavaVMs failed or no JVMs found" );
        return false;
    }

    LOG_INFO ( "Found " << count << " JVM(s)" );

    jint res = m_jvm->GetEnv ( reinterpret_cast < void ** > ( &m_env ), JNI_VERSION_1_6 );
    if ( res == JNI_EDETACHED ) {
        LOG_INFO ( "Thread not attached, attaching..." );
        res = m_jvm->AttachCurrentThread ( reinterpret_cast < void ** > ( &m_env ), nullptr );
    }

    if ( res != JNI_OK || !m_env ) {
        LOG_ERROR ( "Failed to attach to JVM thread (result: " << res << ")" );
        return false;
    }

    LOG_SUCCESS ( "Thread attached successfully" );
    return true;
}

bool c_java::setup_jvmti ( ) {
    if ( !m_jvm ) {
        LOG_ERROR ( "JVM not initialized" );
        return false;
    }

    jint res = m_jvm->GetEnv ( reinterpret_cast < void ** > ( &m_jvmti ), JVMTI_VERSION_1_2 );
    if ( res != JNI_OK || !m_jvmti ) {
        LOG_ERROR ( "Failed to get JVMTI environment" );
        return false;
    }

    // Add capabilities for class redefinition
    jvmtiCapabilities caps = { 0 };
    caps.can_redefine_classes = 1;
    caps.can_retransform_classes = 1;

    jvmtiError err = m_jvmti->AddCapabilities ( &caps );
    if ( err != JVMTI_ERROR_NONE ) {
        LOG_ERROR ( "Failed to add JVMTI capabilities: " << err );
        return false;
    }

    LOG_SUCCESS ( "JVMTI initialized with redefinition capabilities" );
    return true;
}

bool c_java::define_native_bridge_class ( ) {
    if ( !m_jvmti ) {
        LOG_ERROR ( "JVMTI not initialized" );
        return false;
    }

    std::string bridge_class_name = xorstr_ ( "cosmicclient/NativeBridge" );
    jclass existing_class = nullptr;

    jint class_count = 0;
    jclass * classes = nullptr;
    jvmtiError err = m_jvmti->GetLoadedClasses ( &class_count, &classes );
    if ( err == JVMTI_ERROR_NONE ) {
        std::string signature = std::string ( xorstr_ ( "L" ) ) + bridge_class_name + xorstr_ ( ";" );

        for ( jint i = 0; i < class_count; i++ ) {
            char * sig = nullptr;
            m_jvmti->GetClassSignature ( classes [ i ], &sig, nullptr );

            if ( sig && strcmp ( sig, signature.c_str ( ) ) == 0 ) {
                existing_class = classes [ i ];
                LOG_INFO ( "Found existing NativeBridge class (re-injection)" );
                m_jvmti->Deallocate ( reinterpret_cast< unsigned char * > ( sig ) );
                break;
            }

            if ( sig )
                m_jvmti->Deallocate ( reinterpret_cast< unsigned char * >( sig ) );
        }

        m_jvmti->Deallocate ( reinterpret_cast< unsigned char * >( classes ) );
    }

    if ( existing_class ) {
        m_native_bridge_class = static_cast< jclass >( m_env->NewGlobalRef ( existing_class ) );
        LOG_SUCCESS ( "Using existing NativeBridge class" );
    }
    else {
        LOG_INFO ( "NativeBridge class not found, defining new class" );

        const jclass minecraft_cls = get_class ( xorstr_ ( "Minecraft" ) );
        if ( !minecraft_cls ) {
            LOG_ERROR ( "Could not find Minecraft class" );
            return false;
        }

        jobject class_loader = nullptr;
        err = m_jvmti->GetClassLoader ( minecraft_cls, &class_loader );
        if ( err != JVMTI_ERROR_NONE ) {
            LOG_ERROR ( "Failed to get Minecraft classloader: " << err );
            return false;
        }

        if ( !class_loader ) {
            LOG_INFO ( "Minecraft uses bootstrap classloader" );
        } else {
            LOG_SUCCESS ( "Got Minecraft's classloader" );
        }

        if ( native_bridge_class_size == 0 ) {
            LOG_ERROR ( "NativeBridge class data is empty" );
            return false;
        }

        LOG_INFO ( "Loading embedded NativeBridge.class (" << native_bridge_class_size << " bytes)" );

        if ( native_bridge_class_size >= 8 ) {
            const unsigned char major = native_bridge_class [ 6 ];
            const unsigned char minor = native_bridge_class [ 4 ];
            LOG_INFO ( "NativeBridge class version: " << static_cast< int >( major ) << "." << ( int ) minor );
        }

        m_native_bridge_class = m_env->DefineClass (
            bridge_class_name.c_str ( ),
            class_loader,
            reinterpret_cast< const jbyte * >( native_bridge_class ),
            native_bridge_class_size
        );

        if ( !m_native_bridge_class ) {
            LOG_ERROR ( "Failed to define NativeBridge class" );
            if ( m_env->ExceptionCheck ( ) ) {
                m_env->ExceptionDescribe ( );
                m_env->ExceptionClear ( );
            }
            return false;
        }

        m_native_bridge_class = static_cast< jclass >( m_env->NewGlobalRef ( m_native_bridge_class ) );
        LOG_SUCCESS ( "NativeBridge class defined successfully" );
    }

    return true;
}

bool c_java::register_native_bridge_methods ( ) {
    if ( !m_native_bridge_class ) {
        LOG_ERROR ( "NativeBridge class not initialized" );
        return false;
    }

    // Clean slate � important for re-injection and post-redefine re-registration
    m_env->UnregisterNatives ( m_native_bridge_class );

    std::string nm1_name = xorstr_ ( "onRenderEntityStart" );
    std::string nm1_sig = xorstr_ ( "(JJ)V" );
    std::string nm2_name = xorstr_ ( "onRenderEntityEnd" );
    std::string nm2_sig = xorstr_ ( "(JJ)V" );
    std::string nm3_name = xorstr_ ( "onFish" );
    std::string nm3_sig = xorstr_ ( "(DDD)V" );

    const JNINativeMethod methods [ ] = {
        { nm1_name.data ( ), nm1_sig.data ( ), ( void * ) &Native_onRenderEntityStart   },
        { nm2_name.data ( ), nm2_sig.data ( ), ( void * ) &Native_onRenderEntityEnd     },
        { nm3_name.data ( ), nm3_sig.data ( ), ( void * ) &Native_onFish                },
    };

    const jint result = m_env->RegisterNatives ( m_native_bridge_class, methods, 3 );
    if ( result != 0 ) {
        LOG_ERROR ( "Failed to register native methods" );
        if ( m_env->ExceptionCheck ( ) ) {
            m_env->ExceptionDescribe ( );
            m_env->ExceptionClear ( );
        }
        return false;
    }

    LOG_SUCCESS ( "Native methods registered successfully" );

    // Cache setInjected method ID
    m_set_injected_method = m_env->GetStaticMethodID (
        m_native_bridge_class, xorstr_ ( "setInjected" ), xorstr_ ( "(Z)V" ) );

    if ( !m_set_injected_method ) {
        LOG_ERROR ( "Could not find setInjected method" );
        check_exception ( );
        return false;
    }

    LOG_SUCCESS ( "Cached setInjected method ID" );

    m_env->CallStaticVoidMethod ( m_native_bridge_class, m_set_injected_method, JNI_TRUE );
    if ( m_env->ExceptionCheck ( ) ) {
        m_env->ExceptionDescribe ( );
        m_env->ExceptionClear ( );
        return false;
    }

    LOG_SUCCESS ( "Set NativeBridge.injected = true" );
    return true;
}

bool c_java::redefine_class_from_file ( const std::string & class_name, const std::string & file_path ) const {
    if ( !m_jvmti ) {
        LOG_ERROR ( "JVMTI not initialized" );
        return false;
    }

    // Read class file
    std::ifstream file ( file_path, std::ios::binary | std::ios::ate );
    if ( !file.is_open ( ) ) {
        LOG_ERROR ( "Failed to open: " << file_path );
        return false;
    }

    std::streamsize size = file.tellg ( );
    file.seekg ( 0, std::ios::beg );

    std::vector<unsigned char> class_bytes ( size );
    if ( !file.read ( reinterpret_cast< char * >( class_bytes.data ( ) ), size ) ) {
        LOG_ERROR ( "Failed to read file" );
        return false;
    }

    LOG_INFO ( "Read " << class_bytes.size ( ) << " bytes from " << file_path );

    return redefine_class_from_memory ( class_name, class_bytes.data ( ), class_bytes.size ( ) );
}

bool c_java::redefine_class_from_memory ( const std::string & class_name, const unsigned char * class_bytes, size_t size ) const {
    if ( !m_jvmti ) {
        LOG_ERROR ( "JVMTI not initialized" );
        return false;
    }

    // Get all loaded classes
    jint class_count = 0;
    jclass * classes = nullptr;
    jvmtiError err = m_jvmti->GetLoadedClasses ( &class_count, &classes );
    if ( err != JVMTI_ERROR_NONE ) {
        LOG_ERROR ( "Failed to get loaded classes" );
        return false;
    }

    LOG_INFO ( "Searching " << class_count << " loaded classes for: " << class_name );

    // Find target class
    jclass target_class = nullptr;
    const std::string signature = xorstr_ ( "L" ) + class_name + xorstr_ ( ";" );

    for ( jint i = 0; i < class_count; i++ ) {
        char * sig = nullptr;
        m_jvmti->GetClassSignature ( classes [ i ], &sig, nullptr );

        if ( sig && strcmp ( sig, signature.c_str ( ) ) == 0 ) {
            target_class = classes [ i ];
            LOG_SUCCESS ( "Found class: " << sig );
            m_jvmti->Deallocate ( reinterpret_cast < unsigned char * > ( sig ) );
            break;
        }

        if ( sig ) {
            m_jvmti->Deallocate ( reinterpret_cast < unsigned char * > ( sig ) );
        }
    }

    if ( !target_class ) {
        LOG_ERROR ( "Class not found: " << class_name );
        m_jvmti->Deallocate ( reinterpret_cast < unsigned char * > ( classes ) );
        return false;
    }

    // Prepare redefinition
    jvmtiClassDefinition class_def = {};
    class_def.klass = target_class;
    class_def.class_byte_count = static_cast < jint > ( size );
    class_def.class_bytes = class_bytes;

    // Redefine the class
    LOG_INFO ( "Attempting to redefine class: " << class_name );
    err = m_jvmti->RedefineClasses ( 1, &class_def );

    m_jvmti->Deallocate ( reinterpret_cast < unsigned char * > ( classes ) );

    if ( err == JVMTI_ERROR_NONE ) {
        LOG_SUCCESS ( "Class redefined successfully: " << class_name );
        return true;
    }
    else {
        LOG_ERROR ( "Failed to redefine class: " << class_name << " (error: " << err << ")" );

        switch ( err ) {
        case JVMTI_ERROR_UNSUPPORTED_REDEFINITION_METHOD_ADDED:
            LOG_ERROR ( "Cannot add methods" );
            break;
        case JVMTI_ERROR_UNSUPPORTED_REDEFINITION_SCHEMA_CHANGED:
            LOG_ERROR ( "Cannot change fields or method signatures" );
            break;
        case JVMTI_ERROR_INVALID_CLASS_FORMAT:
            LOG_ERROR ( "Invalid class file format" );
            break;
        case JVMTI_ERROR_FAILS_VERIFICATION:
            LOG_ERROR ( "Class verification failed" );
            break;
        case JVMTI_ERROR_NAMES_DONT_MATCH:
            LOG_ERROR ( "Class name doesn't match" );
            break;
        default:
            LOG_ERROR ( "Unknown error" );
            break;
        }

        return false;
    }
}

bool c_java::setup_class_loader ( ) {
    const jclass thread_class = m_env->FindClass ( xorstr_ ( "java/lang/Thread" ) );
    if ( !thread_class ) return false;

    const jmethodID get_thread_class_loader = m_env->GetMethodID ( thread_class, xorstr_ ( "getContextClassLoader" ), xorstr_ ( "()Ljava/lang/ClassLoader;" ) );
    const jmethodID get_thread_id = m_env->GetMethodID ( thread_class, xorstr_ ( "getId" ), xorstr_ ( "()J" ) );
    const jmethodID get_all_threads = m_env->GetStaticMethodID ( thread_class, xorstr_ ( "getAllStackTraces" ), xorstr_ ( "()Ljava/util/Map;" ) );

    const jclass map_class = m_env->FindClass ( xorstr_ ( "java/util/Map" ) );
    const jmethodID get_keyset = m_env->GetMethodID ( map_class, xorstr_ ( "keySet" ), xorstr_ ( "()Ljava/util/Set;" ) );

    const jclass set_class = m_env->FindClass ( xorstr_ ( "java/util/Set" ) );
    const jmethodID get_iterator = m_env->GetMethodID ( set_class, xorstr_ ( "iterator" ), xorstr_ ( "()Ljava/util/Iterator;" ) );

    const jclass iterator_class = m_env->FindClass ( xorstr_ ( "java/util/Iterator" ) );
    const jmethodID has_next = m_env->GetMethodID ( iterator_class, xorstr_ ( "hasNext" ), xorstr_ ( "()Z" ) );
    const jmethodID next = m_env->GetMethodID ( iterator_class, xorstr_ ( "next" ), xorstr_ ( "()Ljava/lang/Object;" ) );

    const jobject threads_map = m_env->CallStaticObjectMethod ( thread_class, get_all_threads );
    const jobject keyset = m_env->CallObjectMethod ( threads_map, get_keyset );
    const jobject iterator = m_env->CallObjectMethod ( keyset, get_iterator );

    // Find main thread (ID = 1)
    while ( m_env->CallBooleanMethod ( iterator, has_next ) ) {
        const jobject thread = m_env->CallObjectMethod ( iterator, next );

        if ( const jlong thread_id = m_env->CallLongMethod ( thread, get_thread_id ); thread_id == 1 ) {
            const jobject loader = m_env->CallObjectMethod ( thread, get_thread_class_loader );
            m_main_thread_class_loader = m_env->NewGlobalRef ( loader );
            m_env->DeleteLocalRef ( thread );
            break;
        }

        m_env->DeleteLocalRef ( thread );
    }

    // Cleanup
    m_env->DeleteLocalRef ( thread_class );
    m_env->DeleteLocalRef ( map_class );
    m_env->DeleteLocalRef ( set_class );
    m_env->DeleteLocalRef ( iterator_class );
    m_env->DeleteLocalRef ( threads_map );
    m_env->DeleteLocalRef ( keyset );
    m_env->DeleteLocalRef ( iterator );

    return m_main_thread_class_loader != nullptr;
}

bool c_java::verify_mappings ( const mappings::mapping_set & mappings ) const {
    if ( mappings.test_class.empty ( ) ) {
        LOG_ERROR ( "No test class specified for " << oof::fg_color ( { 235, 40, 63 } ) << mappings.name.c_str ( ) );
        return false;
    }

    const auto it = mappings.classes.find ( mappings.test_class );
    if ( it == mappings.classes.end ( ) ) {
        LOG_ERROR ( "Test class " << oof::fg_color ( { 235, 40, 63 } ) << mappings.test_class.c_str ( ) << oof::reset_formatting ( ) << " not found in mappings" );
        return false;
    }

    const jclass test = load_class ( it->second.name );
    const bool valid = ( test != nullptr );

    if ( test ) {
        m_env->DeleteLocalRef ( test );
    }

    return valid;
}

void c_java::cache_all_mappings ( ) {
    LOG_INFO ( "Caching all classes, methods, and fields..." );

    // Cache all classes
    for ( const auto & [key, mapping] : m_active_mappings.classes ) {
        if ( const jclass clazz = load_class ( mapping.name ) ) {
            m_classes [ key ] = static_cast< jclass >( m_env->NewGlobalRef ( clazz ) );
            m_env->DeleteLocalRef ( clazz );
        }
        else {
            LOG_ERROR ( "Failed to cache class: " << oof::fg_color ( { 235, 40, 63 } ) << key.c_str ( ) );
        }
    }

    // Cache all methods
    for ( const auto & [key, mapping] : m_active_mappings.methods ) {
        // Key format: "ClassName.methodName"
        const size_t dot = key.find ( '.' );
        if ( dot == std::string::npos ) continue;

        std::string class_key = key.substr ( 0, dot );

        if ( const jclass clazz = get_class ( class_key ) ) {
            std::string signature = replace_placeholders ( mapping.signature );
            const jmethodID method = mapping.is_static
                ? m_env->GetStaticMethodID ( clazz, mapping.name.c_str ( ), signature.c_str ( ) )
                : m_env->GetMethodID ( clazz, mapping.name.c_str ( ), signature.c_str ( ) );

            if ( method ) {
                m_methods [ key ] = method;
            }
            else {
                LOG_ERROR ( "Failed to cache method: " << key.c_str ( ) << " (" << mapping.name.c_str ( ) << ", " << signature.c_str ( ) << ")" );
                check_exception ( );
            }
        }
    }

    // Cache all fields
    for ( const auto & [key, mapping] : m_active_mappings.fields ) {
        const size_t dot = key.find ( '.' );
        if ( dot == std::string::npos ) continue;

        std::string class_key = key.substr ( 0, dot );

        if ( const jclass clazz = get_class ( class_key ) ) {
            std::string signature = replace_placeholders ( mapping.signature );
            const jfieldID field = mapping.is_static
                ? m_env->GetStaticFieldID ( clazz, mapping.name.c_str ( ), signature.c_str ( ) )
                : m_env->GetFieldID ( clazz, mapping.name.c_str ( ), signature.c_str ( ) );

            if ( field ) {
                m_fields [ key ] = field;
            }
            else {
                LOG_ERROR ( "Failed to cache field: " << key.c_str ( ) << " (" << mapping.name.c_str ( ) << ", " << signature.c_str ( ) << ")" );
                check_exception ( );
            }
        }
    }

    LOG_SUCCESS ( "Cached "
        << oof::fg_color ( { 100, 150, 255 } ) << m_classes.size ( ) << oof::reset_formatting ( ) << " classes, "
        << oof::fg_color ( { 100, 150, 255 } ) << m_methods.size ( ) << oof::reset_formatting ( ) << " methods, "
        << oof::fg_color ( { 100, 150, 255 } ) << m_fields.size ( ) << oof::reset_formatting ( ) << " fields" );
}

void c_java::cache_native_bridge_mappings ( ) {
    LOG_INFO ( "Caching NativeBridge methods and fields..." );

    int cached_methods = 0;
    int cached_fields = 0;

    for ( const auto & [key, mapping] : m_active_mappings.methods ) {
        if ( key.find ( xorstr_ ( "NativeBridge." ) ) != 0 ) continue;

        if ( const jclass clazz = get_class ( xorstr_ ( "NativeBridge" ) ) ) {
            std::string signature = replace_placeholders ( mapping.signature );
            const jmethodID method = mapping.is_static
                ? m_env->GetStaticMethodID ( clazz, mapping.name.c_str ( ), signature.c_str ( ) )
                : m_env->GetMethodID ( clazz, mapping.name.c_str ( ), signature.c_str ( ) );

            if ( method ) {
                m_methods [ key ] = method;
                cached_methods++;
            }
            else {
                LOG_ERROR ( "Failed to cache NativeBridge method: " << key.c_str ( ) );
                check_exception ( );
            }
        }
    }

    for ( const auto & [key, mapping] : m_active_mappings.fields ) {
        if ( key.find ( xorstr_ ( "NativeBridge." ) ) != 0 ) continue;

        if ( const jclass clazz = get_class ( xorstr_ ( "NativeBridge" ) ) ) {
            std::string signature = replace_placeholders ( mapping.signature );
            const jfieldID field = mapping.is_static
                ? m_env->GetStaticFieldID ( clazz, mapping.name.c_str ( ), signature.c_str ( ) )
                : m_env->GetFieldID ( clazz, mapping.name.c_str ( ), signature.c_str ( ) );

            if ( field ) {
                m_fields [ key ] = field;
                cached_fields++;
            }
            else {
                LOG_ERROR ( "Failed to cache NativeBridge field: " << key.c_str ( ) );
                check_exception ( );
            }
        }
    }

    LOG_SUCCESS ( "Cached " << cached_methods << " NativeBridge methods, " << cached_fields << " fields" );
}

jclass c_java::load_class ( const std::string & name ) const {
    const jclass loader_class = m_env->FindClass ( xorstr_ ( "java/lang/ClassLoader" ) );
    const jmethodID load_class_method = m_env->GetMethodID ( loader_class, xorstr_ ( "loadClass" ), xorstr_ ( "(Ljava/lang/String;)Ljava/lang/Class;" ) );

    const jstring class_name = m_env->NewStringUTF ( name.c_str ( ) );
    const auto clazz = static_cast< jclass >( m_env->CallObjectMethod ( m_main_thread_class_loader, load_class_method, class_name ) );

    m_env->DeleteLocalRef ( loader_class );
    m_env->DeleteLocalRef ( class_name );

    check_exception ( );
    return clazz;
}

jclass c_java::get_class ( const std::string & key ) {
    const auto it = m_classes.find ( key );
    return ( it != m_classes.end ( ) ) ? it->second : nullptr;
}

jmethodID c_java::get_method ( const std::string & key ) {
    const auto it = m_methods.find ( key );
    return ( it != m_methods.end ( ) ) ? it->second : nullptr;
}

jfieldID c_java::get_field ( const std::string & key ) {
    const auto it = m_fields.find ( key );
    return ( it != m_fields.end ( ) ) ? it->second : nullptr;
}

jobject c_java::get_minecraft ( ) const {
    return m_minecraft;
}

jobject c_java::get_cosmic_client ( ) const {
    return m_cosmic_client;
}

std::string c_java::replace_placeholders ( const std::string & input ) {
    std::string result;
    std::string placeholder;
    bool in_placeholder = false;

    for ( const char c : input ) {
        if ( c == '{' ) {
            in_placeholder = true;
        }
        else if ( c == '}' ) {
            in_placeholder = false;

            if ( auto it = m_active_mappings.classes.find ( placeholder ); it != m_active_mappings.classes.end ( ) ) {
                result += it->second.descriptor;
            }
            placeholder.clear ( );
        }
        else if ( in_placeholder ) {
            placeholder += c;
        }
        else {
            result += c;
        }
    }

    return result;
}

void c_java::check_exception ( ) const {
    if ( m_env->ExceptionCheck ( ) ) {
        m_env->ExceptionDescribe ( );
        m_env->ExceptionClear ( );
    }
}

void c_java::shutdown ( ) {
    LOG_INFO ( "Shutting down Java interface..." );
    free_global_refs ( );
}

// overcomplicated, but only ran once so who cares
void c_java::set_injected_state ( const bool state ) const {
    if ( !m_native_bridge_class || !m_jvm || !m_set_injected_method ) {
        LOG_ERROR ( "Cannot set injected state - invalid state" );
        return;
    }

    // Get JNIEnv for this thread (attach if needed)
    JNIEnv * env = nullptr;
    bool was_attached = false;

    const jint get_env_result = m_jvm->GetEnv ( reinterpret_cast < void ** > ( &env ), JNI_VERSION_1_6 );

    if ( get_env_result == JNI_EDETACHED ) {
        // Thread not attached, attach it
        if ( m_jvm->AttachCurrentThread ( reinterpret_cast < void ** > ( &env ), nullptr ) == JNI_OK ) {
            was_attached = true;
            LOG_INFO ( "Attached current thread to JVM for setInjected call" );
        }
        else {
            LOG_ERROR ( "Failed to attach thread to JVM" );
            return;
        }
    }
    else if ( get_env_result != JNI_OK || !env ) {
        LOG_ERROR ( "Failed to get JNIEnv" );
        return;
    }

    // Now call the method with the correct env
    try {
        env->CallStaticVoidMethod ( m_native_bridge_class, m_set_injected_method, state ? JNI_TRUE : JNI_FALSE );

        if ( env->ExceptionCheck ( ) ) {
            LOG_ERROR ( "Exception calling setInjected" );
            env->ExceptionDescribe ( );
            env->ExceptionClear ( );
        }
        else {
            LOG_SUCCESS ( "Set NativeBridge.injected = " << ( state ? "true" : "false" ) );
        }
    }
    catch ( ... ) {
        LOG_ERROR ( "Exception during set_injected_state" );
    }

    // Detach thread if we attached it
    if ( was_attached ) {
        m_jvm->DetachCurrentThread ( );
        LOG_INFO ( "Detached thread from JVM" );
    }
}

void c_java::free_global_refs ( ) {
    if ( m_native_bridge_class ) {
        m_env->DeleteGlobalRef ( m_native_bridge_class );
        m_native_bridge_class = nullptr;
    }

    if ( m_minecraft ) {
        m_env->DeleteGlobalRef ( m_minecraft );
        m_minecraft = nullptr;
    }

    if ( m_cosmic_client ) {
        m_env->DeleteGlobalRef ( m_cosmic_client );
        m_cosmic_client = nullptr;
    }

    if ( m_main_thread_class_loader ) {
        m_env->DeleteGlobalRef ( m_main_thread_class_loader );
        m_main_thread_class_loader = nullptr;
    }

    for ( const auto & clazz : m_classes | std::views::values ) {
        m_env->DeleteGlobalRef ( clazz );
    }

    m_classes.clear ( );
    m_methods.clear ( );
    m_fields.clear ( );
}
