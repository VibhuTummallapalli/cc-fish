#pragma once
#include "block.h"
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_block_state : public c_java_object {
public:
    c_block_state ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    c_block get_block ( ) {
        const auto & cache = block_state_cache::get ( m_java );
        const jobject block = cache.get_block ? m_java->get_env ( )->CallObjectMethod ( m_obj, cache.get_block ) : nullptr;
        return { block, m_java };
	}
};