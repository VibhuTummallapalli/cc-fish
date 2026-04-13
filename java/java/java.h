#pragma once
#include <algorithm>
#include <jni.h>
#include <jvmti.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "../libraries/xor/xorstr.h"

namespace mappings {
    struct class_mapping {
        std::string name;
        std::string descriptor;
    };

    struct method_mapping {
        std::string name;
        std::string signature;
        bool is_static = false;
    };

    struct field_mapping {
        std::string name;
        std::string signature;
        bool is_static = false;
    };

    struct mapping_set {
        std::string name;
        std::string test_class;
        std::unordered_map<std::string, class_mapping> classes;
        std::unordered_map<std::string, method_mapping> methods;
        std::unordered_map<std::string, field_mapping> fields;
    };


    // Helper to create descriptor from class name
    inline std::string make_descriptor ( const std::string & name ) {
        std::string desc = name;
        std::ranges::replace ( desc, '.', '/' );
        return xorstr_ ( "L" ) + desc + xorstr_ ( ";" );
    }

	void register_native_bridge_mappings ( mapping_set & set );
    void register_cosmic_client_mappings ( mapping_set & set );
}

class c_java {
public:
    c_java ( );
    ~c_java ( ) = default;

    bool initialize ( );
    void shutdown ( );
    void set_injected_state ( bool state ) const;

    // Class redefinition & definition
    bool setup_jvmti ( );
    bool define_native_bridge_class ( );
    bool register_native_bridge_methods ( );
    bool redefine_class_from_file ( const std::string & class_name, const std::string & file_path ) const;
    bool redefine_class_from_memory ( const std::string & class_name, const unsigned char * class_bytes, size_t size ) const;

    // Accessors
    jclass get_class ( const std::string & key );
    jmethodID get_method ( const std::string & key );
    jfieldID get_field ( const std::string & key );
    [[nodiscard]] jobject get_minecraft ( ) const;
    [[nodiscard]] jobject get_cosmic_client ( ) const;
    [[nodiscard]] JNIEnv * get_env ( ) const { return m_env; }
    [[nodiscard]] jvmtiEnv * get_jvmti ( ) const { return m_jvmti; }

    [[nodiscard]] jclass get_native_bridge_class ( ) const { return m_native_bridge_class; }
private:
    JNIEnv * m_env;
    jvmtiEnv * m_jvmti;
    JavaVM * m_jvm;
    jobject m_main_thread_class_loader;
    jobject m_minecraft;
    jobject m_cosmic_client;
    jclass m_native_bridge_class;
    jmethodID m_set_injected_method;

    std::unordered_map<std::string, jclass> m_classes;
    std::unordered_map<std::string, jmethodID> m_methods;
    std::unordered_map<std::string, jfieldID> m_fields;
    mappings::mapping_set m_active_mappings;

    bool attach_to_jvm ( );
    bool setup_class_loader ( );
    [[nodiscard]] bool verify_mappings ( const mappings::mapping_set & mappings ) const;
    [[nodiscard]] jclass load_class ( const std::string & name ) const;
    void cache_all_mappings ( );
    void cache_native_bridge_mappings ( );
    void check_exception ( ) const;
    void free_global_refs ( );
    std::string replace_placeholders ( const std::string & input );
};