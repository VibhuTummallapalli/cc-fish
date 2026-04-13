#pragma once
#include "item.h"
#include "cosmic_client_set.h"
#include "nbt_tag_compound.h"
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_itemstack : public c_java_object {
public:
    c_itemstack ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    c_item get_item ( ) {
        const auto & cache = itemstack_cache::get ( m_java );
        const jobject item = cache.item ? m_java->get_env ( )->GetObjectField ( m_obj, cache.item ) : nullptr;
        return { item, m_java };
    }

    [[nodiscard]] int get_item_count ( ) const {
        const auto & cache = itemstack_cache::get ( m_java );
        return cache.stack_size ? m_java->get_env ( )->GetIntField ( m_obj, cache.stack_size ) : 0;
    }

    [[nodiscard]] int get_item_damage ( ) const {
        const auto & cache = itemstack_cache::get ( m_java );
        return cache.get_item_damage ? m_java->get_env ( )->CallIntMethod ( m_obj, cache.get_item_damage ) : 0;
    }

    c_cosmic_client_set get_cosmic_client_set ( ) {
        const auto & cache = itemstack_cache::get ( m_java );
        const jobject set = cache.set ? m_java->get_env ( )->GetObjectField ( m_obj, cache.set ) : nullptr;
        return { set, m_java };
	}

    c_nbt_tag_compound get_nbt_compound ( ) {
		const auto & cache = itemstack_cache::get ( m_java );
        const jobject nbt_compound = cache.get_nbt_compound ? m_java->get_env ( )->GetObjectField ( m_obj, cache.get_nbt_compound ) : nullptr;
		return { nbt_compound, m_java };
	}
};