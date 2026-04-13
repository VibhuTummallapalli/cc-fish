#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_mouse_helper : public c_java_object {
public:
    c_mouse_helper ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    void grab_mouse_cursor ( ) const {
        const auto & cache = mouse_helper_cache::get ( m_java );
        if ( cache.grab_mouse_cursor ) m_java->get_env ( )->CallVoidMethod ( m_obj, cache.grab_mouse_cursor );
    }

    void ungrab_mouse_cursor ( ) const {
        const auto & cache = mouse_helper_cache::get ( m_java );
        if ( cache.ungrab_mouse_cursor ) m_java->get_env ( )->CallVoidMethod ( m_obj, cache.ungrab_mouse_cursor );
    }
};