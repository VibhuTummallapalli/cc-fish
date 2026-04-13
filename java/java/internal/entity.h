#pragma once
#include "abstract_client_player.h"
#include "axis_aligned_bb.h"
#include "inventory_player.h"
#include "itemstack.h"
#include "mc_vector.h"
#include "potion_effect.h"
#include "uuid.h"
#include "../../math/vec.h"
#include "cache/cached.h"
#include "java_object/java_object.h"

struct entity_collection_cache {
    jclass map_class = nullptr;
    jmethodID map_size = nullptr;
    jmethodID map_entry_set = nullptr;
    jclass set_class = nullptr;
    jmethodID set_iterator = nullptr;
    jclass iterator_class = nullptr;
    jmethodID iterator_has_next = nullptr;
    jmethodID iterator_next = nullptr;
    jclass entry_class = nullptr;
    jmethodID entry_get_value = nullptr;
    bool ready = false;

    void init ( JNIEnv * env ) {
        if ( ready ) return;

        map_class = static_cast< jclass >(
            env->NewGlobalRef ( env->FindClass ( "java/util/Map" ) ) );
        map_size = env->GetMethodID ( map_class, "size", "()I" );
        map_entry_set = env->GetMethodID ( map_class, "entrySet", "()Ljava/util/Set;" );

        set_class = static_cast< jclass >(
            env->NewGlobalRef ( env->FindClass ( "java/util/Set" ) ) );
        set_iterator = env->GetMethodID ( set_class, "iterator", "()Ljava/util/Iterator;" );

        iterator_class = static_cast< jclass >(
            env->NewGlobalRef ( env->FindClass ( "java/util/Iterator" ) ) );
        iterator_has_next = env->GetMethodID ( iterator_class, "hasNext", "()Z" );
        iterator_next = env->GetMethodID ( iterator_class, "next", "()Ljava/lang/Object;" );

        entry_class = static_cast< jclass >(
            env->NewGlobalRef ( env->FindClass ( "java/util/Map$Entry" ) ) );
        entry_get_value = env->GetMethodID ( entry_class, "getValue", "()Ljava/lang/Object;" );

        ready = true;
    }
};

// Single global instance
inline entity_collection_cache g_jni_entity_collections;

class c_entity : public c_java_object {
public:
    c_entity ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] std::string get_name ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        if ( !cache.get_name || !m_obj ) return "";

        const auto jstr = static_cast < jstring > ( m_java->get_env ( )->CallObjectMethod ( m_obj, cache.get_name, 0L, static_cast < jchar > ( 0 ) ) );
        if ( !jstr ) return "";

        const char * str = m_java->get_env ( )->GetStringUTFChars ( jstr, nullptr );
        std::string result ( str );
        m_java->get_env (  )->ReleaseStringUTFChars ( jstr, str );
        m_java->get_env (  )->DeleteLocalRef ( jstr );
        return result;
    }

    // Position getters
    [[nodiscard]] double get_pos_x ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.pos_x ? m_java->get_env (  )->GetDoubleField ( m_obj, cache.pos_x ) : 0.0;
    }
    [[nodiscard]] double get_pos_y ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.pos_y ? m_java->get_env (  )->GetDoubleField ( m_obj, cache.pos_y ) : 0.0;
    }
    [[nodiscard]] double get_pos_z ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.pos_z ? m_java->get_env (  )->GetDoubleField ( m_obj, cache.pos_z ) : 0.0;
    }
    [[nodiscard]] vec3<double> get_pos ( ) const {
        return { get_pos_x ( ), get_pos_y ( ), get_pos_z ( ) };
	}

    [[nodiscard]] double get_last_pos_x ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.last_pos_x ? m_java->get_env (  )->GetDoubleField ( m_obj, cache.last_pos_x ) : 0.0;
    }
    [[nodiscard]] double get_last_pos_y ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.last_pos_y ? m_java->get_env (  )->GetDoubleField ( m_obj, cache.last_pos_y ) : 0.0;
    }
    [[nodiscard]] double get_last_pos_z ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.last_pos_z ? m_java->get_env (  )->GetDoubleField ( m_obj, cache.last_pos_z ) : 0.0;
    }
    [[nodiscard]] vec3<double> get_last_pos () const {
        return { get_last_pos_x ( ), get_last_pos_y ( ), get_last_pos_z ( ) };
	}

    // Motion getters/setters
    [[nodiscard]] double get_motion_x ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.motion_x ? m_java->get_env (  )->GetDoubleField ( m_obj, cache.motion_x ) : 0.0;
    }
    [[nodiscard]] double get_motion_y ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.motion_y ? m_java->get_env (  )->GetDoubleField ( m_obj, cache.motion_y ) : 0.0;
    }
    [[nodiscard]] double get_motion_z ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.motion_z ? m_java->get_env (  )->GetDoubleField ( m_obj, cache.motion_z ) : 0.0;
    }
    [[nodiscard]] vec3<double> get_motion () const {
		return { get_motion_x ( ), get_motion_y ( ), get_motion_z ( ) };
    }

    void set_motion_x ( const double val ) const {
        const auto & cache = entity_cache::get ( m_java );
        if ( cache.motion_x ) m_java->get_env (  )->SetDoubleField ( m_obj, cache.motion_x, val );
    }
    void set_motion_y ( const double val ) const {
        const auto & cache = entity_cache::get ( m_java );
        if ( cache.motion_y ) m_java->get_env (  )->SetDoubleField ( m_obj, cache.motion_y, val );
    }
    void set_motion_z ( const double val ) const {
        const auto & cache = entity_cache::get ( m_java );
        if ( cache.motion_z ) m_java->get_env (  )->SetDoubleField ( m_obj, cache.motion_z, val );
    }

    // Rotation
    [[nodiscard]] float get_rotation_yaw ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.rotation_yaw ? m_java->get_env (  )->GetFloatField ( m_obj, cache.rotation_yaw ) : 0.0f;
    }
    [[nodiscard]] float get_rotation_pitch ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.rotation_pitch ? m_java->get_env (  )->GetFloatField ( m_obj, cache.rotation_pitch ) : 0.0f;
    }
    [[nodiscard]] float get_prev_rotation_yaw ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.prev_rotation_yaw ? m_java->get_env (  )->GetFloatField ( m_obj, cache.prev_rotation_yaw ) : 0.0f;
    }
    [[nodiscard]] float get_prev_rotation_pitch ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.prev_rotation_pitch ? m_java->get_env (  )->GetFloatField ( m_obj, cache.prev_rotation_pitch ) : 0.0f;
    }
    [[nodiscard]] vec3<float> get_angles ( ) const {
		return { get_rotation_yaw ( ), get_rotation_pitch ( ), 0.0f };
    }

    void set_rotation_yaw ( const float val ) const {
        const auto & cache = entity_cache::get ( m_java );
        if ( cache.rotation_yaw ) {
            const auto cur_yaw = get_rotation_yaw ( );
            m_java->get_env ( )->SetFloatField ( m_obj, cache.prev_rotation_yaw, cur_yaw );
            m_java->get_env ( )->SetFloatField ( m_obj, cache.rotation_yaw, val );
        }
	}
    void set_rotation_pitch ( const float val ) const {
        const auto & cache = entity_cache::get ( m_java );
        if ( cache.rotation_pitch ) {
            const auto cur_pitch = get_rotation_pitch ( );
            m_java->get_env ( )->SetFloatField ( m_obj, cache.prev_rotation_pitch, cur_pitch );
            m_java->get_env ( )->SetFloatField ( m_obj, cache.rotation_pitch, val );
        }
    }

    // Other fields
    [[nodiscard]] int get_entity_id ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.entity_id ? m_java->get_env (  )->GetIntField ( m_obj, cache.entity_id ) : 0;
    }
    [[nodiscard]] int get_ticks_existed ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.ticks_existed ? m_java->get_env (  )->GetIntField ( m_obj, cache.ticks_existed ) : 0;
    }
    [[nodiscard]] int get_hurt_time ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.hurt_time ? m_java->get_env (  )->GetIntField ( m_obj, cache.hurt_time ) : 0;
    }

    [[nodiscard]] bool is_in_web ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.is_in_web ? m_java->get_env (  )->GetBooleanField ( m_obj, cache.is_in_web ) : false;
    }

    // Methods
    [[nodiscard]] bool is_invisible ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.is_invisible ? m_java->get_env (  )->CallBooleanMethod ( m_obj, cache.is_invisible ) : false;
    }
    [[nodiscard]] bool is_entity_alive ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.is_entity_alive ? m_java->get_env (  )->CallBooleanMethod ( m_obj, cache.is_entity_alive ) : false;
    }

    void set_position ( const double x, const double y, const double z ) const {
        const auto & cache = entity_cache::get ( m_java );
        if ( cache.set_position ) m_java->get_env (  )->CallVoidMethod ( m_obj, cache.set_position, x, y, z );
    }

    [[nodiscard]] float get_health ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.get_health ? m_java->get_env (  )->CallFloatMethod ( m_obj, cache.get_health ) : 0.0f;
    }

    [[nodiscard]] float get_max_health ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.get_max_health ? m_java->get_env (  )->CallFloatMethod ( m_obj, cache.get_max_health ) : 0.0f;
    }

    // double jump
    void double_jump ( ) const {
		const auto & cache = entity_player_cache::get ( m_java );
        if ( cache.double_jump ) m_java->get_env ( )->CallVoidMethod ( m_obj, cache.double_jump );
    }

    // swing item
    void swing_item ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        if ( cache.swing_item ) m_java->get_env ( )->CallVoidMethod ( m_obj, cache.swing_item );
    }

    // set sprinting
    void set_sprinting ( const bool state ) const { //IZCS
	    const auto & cache = entity_cache::get ( m_java );
		if ( cache.set_sprinting ) m_java->get_env ( )->CallVoidMethod ( m_obj, cache.set_sprinting, static_cast < jint > ( 0 ), static_cast < jboolean > ( state ), static_cast < jchar > ( 0 ), static_cast < jshort > ( 0 ) );
    }

    // get bounding box object
    [[nodiscard]] c_axis_aligned_bb get_bounding_box ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        const jobject bounding_box = cache.bounding_box ? m_java->get_env ( )->GetObjectField ( m_obj, cache.bounding_box ) : nullptr;
		return { bounding_box, m_java };
	}

    // get uuid
    [[nodiscard]] c_uuid get_uuid ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        jobject uuid = cache.uuid ? m_java->get_env (  )->CallObjectMethod ( m_obj, cache.uuid ) : nullptr;
        return { uuid, m_java };
    }

    // get held item
    [[nodiscard]] c_itemstack get_held_item ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        jobject item_stack = cache.get_held_item ? m_java->get_env (  )->CallObjectMethod ( m_obj, cache.get_held_item ) : nullptr;
        return { item_stack, m_java };
	}

    // get equipment in slot
    [[nodiscard]] c_itemstack get_equipment_in_slot ( const int slot ) const {
        const auto & cache = entity_cache::get ( m_java );
        jobject item_stack = cache.get_equipment_in_slot ? m_java->get_env ( )->CallObjectMethod ( m_obj, cache.get_equipment_in_slot, slot ) : nullptr;
        return { item_stack, m_java };
    }

    // get abstract client player (assume entity = player)
    [[nodiscard]] c_abstract_client_player get_abstract_player ( ) const {
        return { m_obj, m_java, false };
	}

	// get armor configuration (returns -1 if invalid/mixed, else [set_id in upper bits][omni bit in bit 0])
    [[nodiscard]] int get_armor_configuration ( ) const {
	    constexpr int OMNI_ID = 24; // PVP_OMNI_HACK id
        int set_ids [ 4 ] = { -1, -1, -1, -1 }; // boots, pants, chest, helmet

        // Gather set IDs for all 4 slots
        for ( int slot = 1; slot <= 4; slot++ ) {
	        if ( auto equipment = get_equipment_in_slot ( slot ); equipment.is_valid ( ) ) {
		        if ( auto set = equipment.get_cosmic_client_set ( ); set.is_valid ( ) ) {
                    set_ids [ slot - 1 ] = set.get_id ( );
                }
            }
        }

        // Check if all 4 pieces are present
        for ( const int set_id : set_ids ) {
            if ( set_id == -1 ) {
                return -1; // Missing armor piece
            }
        }

        // Find the non-omni set (if any) and count omni pieces
        int primary_set = -1;
        bool has_omni = false;

        for ( const int set_id : set_ids ) {
            if ( set_id == OMNI_ID ) {
                has_omni = true;
            }
            else {
                if ( primary_set == -1 ) {
                    primary_set = set_id;
                }
                else if ( primary_set != set_id ) {
                    return -1; // Mixed non-omni sets (invalid)
                }
            }
        }

        // All omni
        if ( primary_set == -1 ) {
            return ( OMNI_ID << 1 ); // 48, no omni bit set since it's pure omni
        }

        // Build result: [set_id in upper bits][omni bit in bit 0]
        int result = ( primary_set << 1 );
        if ( has_omni ) {
            result |= 1; // Set omni bit
        }

        return result;
    }

	// get eye position
    [[nodiscard]] vec3<double> get_eye_pos ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        if ( !cache.get_position_eyes ) return { 0.0, 0.0, 0.0 };
        const jobject vec3_obj = m_java->get_env (  )->CallObjectMethod ( m_obj, cache.get_position_eyes, 1.0f );
        if ( !vec3_obj ) return { 0.0, 0.0, 0.0 };

		const auto eye_pos_vec3 = c_mc_vector ( vec3_obj, m_java );

        return eye_pos_vec3.get_vec3( );
	}

    // get inventory
    [[nodiscard]] c_inventory_player get_inventory ( ) const {
        const auto & cache = entity_player_cache::get ( m_java );
        jobject inventory_obj = cache.inventory ? m_java->get_env (  )->GetObjectField ( m_obj, cache.inventory ) : nullptr;
        return { inventory_obj, m_java };
	}

    // get item in use count
    [[nodiscard]] int get_item_in_use_count ( ) const {
        const auto & cache = entity_player_cache::get ( m_java );
        return cache.item_in_use_count ? m_java->get_env (  )->GetIntField ( m_obj, cache.item_in_use_count ) : 0;
	}

    [[nodiscard]] bool is_item_in_use ( ) const {
        const auto & cache = entity_player_cache::get ( m_java );
        if ( !cache.item_in_use ) return false;

        // RAII should auto delete this fella
        const auto item = c_itemstack ( m_java->get_env ( )->GetObjectField ( m_obj, cache.item_in_use ), m_java );

        return item.is_valid ( );
    }

	// remove potion effect by id
    void remove_potion_effect_client ( const int effect_id ) const {
	    const auto & cache = entity_cache::get ( m_java );
        if ( cache.remove_active_potion_effect ) m_java->get_env ( )->CallVoidMethod ( m_obj, cache.remove_active_potion_effect, effect_id );
    }

    // get the map of active potion effects
    [[nodiscard]] std::vector<c_potion_effect> get_potion_effects ( ) {
        std::vector<c_potion_effect> result;
        JNIEnv * env = m_java->get_env ( );
        const auto & cache = entity_cache::get ( m_java );

        if ( !cache.active_potion_map ) return result;

        const jobject map = env->GetObjectField ( m_obj, cache.active_potion_map );
        if ( !map ) return result;

        g_jni_entity_collections.init ( env );

        const jint size = env->CallIntMethod ( map, g_jni_entity_collections.map_size );
        if ( size <= 0 ) {
            env->DeleteLocalRef ( map );
            return result;
        }

        result.reserve ( size );

        const jobject entry_set = env->CallObjectMethod ( map, g_jni_entity_collections.map_entry_set );
        if ( !entry_set ) {
            env->DeleteLocalRef ( map );
            return result;
        }

        const jobject iterator = env->CallObjectMethod ( entry_set, g_jni_entity_collections.set_iterator );
        if ( !iterator ) {
            env->DeleteLocalRef ( entry_set );
            env->DeleteLocalRef ( map );
            return result;
        }

        while ( env->CallBooleanMethod ( iterator, g_jni_entity_collections.iterator_has_next ) ) {
            jobject entry = env->CallObjectMethod ( iterator, g_jni_entity_collections.iterator_next );
            if ( entry ) {
                jobject potion_effect = env->CallObjectMethod ( entry, g_jni_entity_collections.entry_get_value );
                if ( potion_effect ) {
                    result.emplace_back ( potion_effect, m_java );
                }
                env->DeleteLocalRef ( entry );
            }
        }

        env->DeleteLocalRef ( iterator );
        env->DeleteLocalRef ( entry_set );
        env->DeleteLocalRef ( map );

        return result;
    }

    // Type checking
    [[nodiscard]] bool is_instance_of ( const std::string & class_key ) const {
        const jclass clazz = m_java->get_class ( class_key );
        return clazz ? m_java->get_env (  )->IsInstanceOf ( m_obj, clazz ) : false;
    }

    [[nodiscard]] bool is_player ( ) const {
        const auto & cache = entity_player_cache::get ( m_java );
		return cache.entity_player_class ? m_java->get_env ( )->IsInstanceOf ( m_obj, cache.entity_player_class ) : false;
    }

	[[nodiscard]] bool is_ender_pearl ( ) const {
		const auto & cache = entity_ender_pearl_cache::get ( m_java );
		return cache.entity_ender_pearl_class ? m_java->get_env ( )->IsInstanceOf ( m_obj, cache.entity_ender_pearl_class ) : false;
    }

    [[nodiscard]] bool is_potion ( ) const {
        const auto & cache = entity_potion_cache::get ( m_java );
		return cache.entity_potion_class ? m_java->get_env ( )->IsInstanceOf ( m_obj, cache.entity_potion_class ) : false;
    }

	[[nodiscard]] bool is_living ( ) const {
        const auto & cache = entity_cache::get ( m_java );
        return cache.entity_living_base_class ? m_java->get_env ( )->IsInstanceOf ( m_obj, cache.entity_living_base_class ) : false;
    }

    [[nodiscard]] bool is_fishing_bobber ( ) const {
        const auto & cache = entity_fish_hook_cache::get ( m_java );
        return cache.entity_fish_hook_class ? m_java->get_env ( )->IsInstanceOf ( m_obj, cache.entity_fish_hook_class ) : false;
    }
};