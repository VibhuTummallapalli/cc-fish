#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"
#include "resource_location.h"
#include "texture_object.h"

class c_texture_manager : public c_java_object {
private:
    struct map_methods_cache {
        jmethodID map_get = nullptr;
        void init ( JNIEnv * env ) {
            if ( map_get ) return; // Already initialized
            const jclass map_class = env->FindClass ( xorstr_ ( "java/util/Map" ) );
            map_get = env->GetMethodID ( map_class, xorstr_ ( "get" ), xorstr_ ( "(Ljava/lang/Object;)Ljava/lang/Object;" ) );
            env->DeleteLocalRef ( map_class );
        }
    };
    mutable map_methods_cache m_map_methods;
public:
    c_texture_manager ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }
    // Default destructor
    ~c_texture_manager ( ) = default;
    // Get the mapTextureObjects HashMap
    [[nodiscard]] jobject get_map_texture_objects ( ) const {
        const auto & cache = texture_manager_cache::get ( m_java );
        if ( !cache.map_texture_objects ) return nullptr;
        return m_java->get_env ( )->GetObjectField ( m_obj, cache.map_texture_objects );
    }
    // Get the mapTextureCounters HashMap
    [[nodiscard]] jobject get_map_texture_counters ( ) const {
        const auto & cache = texture_manager_cache::get ( m_java );
        if ( !cache.map_texture_counters ) return nullptr;
        return m_java->get_env ( )->GetObjectField ( m_obj, cache.map_texture_counters );
    }
    // Get GLint texture ID for a ResourceLocation
    [[nodiscard]] int get_texture_id_for_resource ( const c_resource_location & resource_location ) const {
        if ( !resource_location.get ( ) ) return -1;
        const auto env = m_java->get_env ( );
        m_map_methods.init ( env );
        // Query directly from Java map
        const jobject map = get_map_texture_objects ( );
        if ( !map ) return -1;
        // Get TextureObject from map
        const jobject texture_obj = env->CallObjectMethod ( map, m_map_methods.map_get, resource_location.get ( ) );
        if ( !texture_obj ) return -1;
        // Get glTextureId
        const c_texture_object tex_obj ( texture_obj, m_java, true );
        return tex_obj.get_gl_texture_id ( );
    }
    // Set GLint texture ID for a ResourceLocation
    bool set_texture_id_for_resource ( const c_resource_location & resource_location, const int texture_id ) const {
        if ( !resource_location.get ( ) ) return false;
        const auto env = m_java->get_env ( );
        m_map_methods.init ( env );
        // Query directly from Java map
        const jobject map = get_map_texture_objects ( );
        if ( !map ) return false;
        // Get TextureObject from map
        const jobject texture_obj = env->CallObjectMethod ( map, m_map_methods.map_get, resource_location.get ( ) );
        if ( !texture_obj ) return false;
        // Get the c_texture_object wrapper
        c_texture_object tex_obj ( texture_obj, m_java, true );

        // Get the abstract texture
        const auto abstract_texture = tex_obj.get_abstract_texture ( );
        if ( !abstract_texture ) return false;

        // Set the texture ID
        abstract_texture.set_texture_id ( texture_id );

        return true;
    }
    // Bind a texture (wrapper for convenience)
    void bind_texture ( const jobject resource_location ) const {
        const auto & cache = texture_manager_cache::get ( m_java );
        if ( cache.bind_texture ) {
            m_java->get_env ( )->CallVoidMethod ( m_obj, cache.bind_texture, resource_location );
        }
    }
};