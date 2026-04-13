#pragma once
#include "itemstack.h"
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_inventory_player : public c_java_object {
public:
    c_inventory_player ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] int get_active_slot ( ) const {
        const auto & cache = inventory_player_cache::get ( m_java );
        return cache.active_slot ? m_java->get_env ( )->GetIntField ( m_obj, cache.active_slot ) : 0;
    }

    void set_active_slot ( const int index ) const {
        const auto & cache = inventory_player_cache::get ( m_java );
		if ( cache.active_slot ) m_java->get_env ( )->SetIntField ( m_obj, cache.active_slot, static_cast < jint > ( index ) );
    }

    [[nodiscard]] int get_inventory_size ( ) const {
        const auto & cache = inventory_player_cache::get ( m_java );
        return cache.get_inventory_size ? m_java->get_env ( )->CallIntMethod ( m_obj, cache.get_inventory_size ) : 0;
    }

    [[nodiscard]] c_itemstack get_current_item ( ) const {
	    const auto & cache = inventory_player_cache::get ( m_java );
        const jobject itemstack = cache.current_item ? m_java->get_env ( )->GetObjectField ( m_obj, cache.current_item ) : nullptr;
		return { itemstack, m_java };
    }

    [[nodiscard]] c_itemstack get_itemstack ( const int index ) const {
        const auto & cache = inventory_player_cache::get ( m_java );
        const jobject itemstack = cache.get_item_stack ? m_java->get_env ( )->CallObjectMethod ( m_obj, cache.get_item_stack, static_cast < jint > ( index ) ) : nullptr;
        return { itemstack, m_java };
	}

	[[nodiscard]] c_item get_item ( const int index ) const {
        return get_itemstack ( index ).get_item ( );
    }

    void set_inventory_slot_contents ( const c_itemstack & item, const int slot ) const {
        const auto & cache = inventory_player_cache::get ( m_java );
		if ( cache.set_item_stack ) m_java->get_env ( )->CallVoidMethod ( m_obj, cache.set_item_stack, static_cast < jlong > ( 0 ), static_cast < jint > ( slot ), static_cast < jbyte > ( 0 ), item.get ( ) );
    }

    void remove_stack_from_slot ( const int slot ) const {
        const auto & cache = inventory_player_cache::get ( m_java );
        if ( cache.remove_stack_from_slot ) m_java->get_env ( )->CallObjectMethod ( m_obj, cache.remove_stack_from_slot, static_cast < jint > ( slot ) );
	}

    void set_item_stack ( const c_itemstack & item ) const {
        const auto & cache = inventory_player_cache::get ( m_java );
        if ( cache.set_item_stack ) m_java->get_env ( )->CallVoidMethod ( m_obj, cache.set_item_stack, item.get ( ) );
	}
};