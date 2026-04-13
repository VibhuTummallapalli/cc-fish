#pragma once
#include <mutex>
#include <optional>
#include "block_pos.h"
#include "block_state.h"
#include "entity.h"
#include "moving_object_position.h"
#include "java_object/java_object.h"

struct world_jni_cache {
    jclass list_class = nullptr;
    jmethodID list_size = nullptr;
    jmethodID list_get = nullptr;
    jclass set_class = nullptr;
    jmethodID set_size = nullptr;
    jmethodID set_iterator = nullptr;
    jclass iterator_class = nullptr;
    jmethodID iterator_has_next = nullptr;
    jmethodID iterator_next = nullptr;
    volatile bool ready = false;

    void init ( JNIEnv * env ) {
        if ( ready ) return;

        jclass lc = env->FindClass ( xorstr_ ( "java/util/List" ) );
        list_class = static_cast< jclass >( env->NewGlobalRef ( lc ) );
        list_size = env->GetMethodID ( list_class, xorstr_ ( "size" ), xorstr_ ( "()I" ) );
        list_get = env->GetMethodID ( list_class, xorstr_ ( "get" ), xorstr_ ( "(I)Ljava/lang/Object;" ) );
        env->DeleteLocalRef ( lc );

        jclass sc = env->FindClass ( xorstr_ ( "java/util/Set" ) );
        set_class = static_cast< jclass >( env->NewGlobalRef ( sc ) );
        set_size = env->GetMethodID ( set_class, xorstr_ ( "size" ), xorstr_ ( "()I" ) );
        set_iterator = env->GetMethodID ( set_class, xorstr_ ( "iterator" ), xorstr_ ( "()Ljava/util/Iterator;" ) );
        env->DeleteLocalRef ( sc );

        jclass ic = env->FindClass ( xorstr_ ( "java/util/Iterator" ) );
        iterator_class = static_cast< jclass >( env->NewGlobalRef ( ic ) );
        iterator_has_next = env->GetMethodID ( iterator_class, xorstr_ ( "hasNext" ), xorstr_ ( "()Z" ) );
        iterator_next = env->GetMethodID ( iterator_class, xorstr_ ( "next" ), xorstr_ ( "()Ljava/lang/Object;" ) );
        env->DeleteLocalRef ( ic );

        ready = true;
    }
};

inline world_jni_cache g_world_jni_cache;

class c_world : public c_java_object {
public:
    c_world ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    std::vector<c_entity> get_player_entities ( ) {
        std::vector<c_entity> result;
        JNIEnv * env = m_java->get_env ( );

        const auto & cache = world_cache::get ( m_java );
        if ( !cache.player_entities ) return result;

        const jobject list = env->GetObjectField ( m_obj, cache.player_entities );
        if ( !list ) return result;

        g_world_jni_cache.init ( env );

        const jint size = env->CallIntMethod ( list, g_world_jni_cache.list_size );
        if ( size <= 0 ) {
            env->DeleteLocalRef ( list );
            return result;
        }

        result.reserve ( size );

        for ( jint i = 0; i < size; ++i ) {
            if ( jobject entity = env->CallObjectMethod ( list, g_world_jni_cache.list_get, i ) ) {
                result.emplace_back ( entity, m_java );
            }
        }

        env->DeleteLocalRef ( list );
        return result;
    }

    std::vector<c_entity> get_entities ( ) {
        std::vector<c_entity> result;
        JNIEnv * env = m_java->get_env ( );

        const auto & cache = world_cache::get ( m_java );
        if ( !cache.entities ) return result;

        const jobject set = env->GetObjectField ( m_obj, cache.entities );
        if ( !set ) return result;

        g_world_jni_cache.init ( env );

        const jint size = env->CallIntMethod ( set, g_world_jni_cache.set_size );
        if ( size <= 0 ) {
            env->DeleteLocalRef ( set );
            return result;
        }
        result.reserve ( size );

        const jobject iterator = env->CallObjectMethod ( set, g_world_jni_cache.set_iterator );
        if ( !iterator ) {
            env->DeleteLocalRef ( set );
            return result;
        }

        while ( env->CallBooleanMethod ( iterator, g_world_jni_cache.iterator_has_next ) ) {
            if ( jobject entity = env->CallObjectMethod ( iterator, g_world_jni_cache.iterator_next ) ) {
                result.emplace_back ( entity, m_java );
            }
        }

        env->DeleteLocalRef ( iterator );
        env->DeleteLocalRef ( set );
        return result;
    }

    [[nodiscard]] long long get_world_time ( ) const {
        const auto & cache = world_cache::get ( m_java );
        return cache.get_world_time ? m_java->get_env ( )->CallLongMethod ( m_obj, cache.get_world_time ) : 0L;
    }

    [[nodiscard]] long long get_total_world_time () const {
        const auto & cache = world_cache::get ( m_java );
        return cache.get_total_world_time ? m_java->get_env ( )->CallLongMethod ( m_obj, cache.get_total_world_time ) : 0L;
    }

    [[nodiscard]] std::optional<c_moving_object_position> ray_trace ( const vec3<double> & start, const vec3<double> & end ) const {
	    const auto & cache = world_cache::get ( m_java );
        if ( !cache.ray_trace ) return std::nullopt;
        c_mc_vector mc_vec( nullptr, m_java );

		const auto start_mv = mc_vec.from_vector ( start );
		const auto end_mv = mc_vec.from_vector ( end );

        // set.methods [ xorstr_ ( "World.rayTrace" ) ] = { xorstr_ ( "a" ), xorstr_ ( "({Vec3}{Vec3}SCZZIZ){MovingObjectPosition}" ), false };
		const auto ray_hit = m_java->get_env ( )->CallObjectMethod ( m_obj, cache.ray_trace, start_mv.get ( ), end_mv.get ( ), 24360, 1, JNI_FALSE, JNI_FALSE, 12999479, JNI_TRUE );

		return ray_hit ? std::optional<c_moving_object_position> ( c_moving_object_position ( ray_hit, m_java ) ) : std::nullopt;
    }

    c_block_state get_block_state ( const vec3 < double > & pos ) {
        const auto & cache = world_cache::get ( m_java );
        const auto block_pos = c_block_pos::create_block_pos ( pos.x, pos.y, pos.z, m_java );
        const jobject state = cache.get_block_state ? m_java->get_env ( )->CallObjectMethod ( m_obj, cache.get_block_state, static_cast < jlong > ( 0 ), block_pos.get (  ), static_cast < jbyte > ( 0 ) ) : nullptr;
        return { state, m_java };
    }
};