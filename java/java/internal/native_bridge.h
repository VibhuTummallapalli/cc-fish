#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_native_bridge {
private:
    c_java * m_java;

public:
    explicit c_native_bridge ( c_java * java ) : m_java ( java ) { }

    [[nodiscard]] bool is_injected ( ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        return cache.is_injected
            ? m_java->get_env ( )->CallStaticBooleanMethod ( cache.native_bridge_class, cache.is_injected ) == JNI_TRUE
            : false;
    }

    void set_clear_lava ( const bool enabled ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_clear_lava ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_clear_lava, enabled ? JNI_TRUE : JNI_FALSE );
        }
    }

    void set_reach_enabled ( const bool enabled ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_has_reach ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_has_reach, enabled ? JNI_TRUE : JNI_FALSE );
        }
	}

    void set_reach_value ( const float value ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_reach_value ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_reach_value, value );
        }
	}

    void set_sprint_cancel ( const bool enabled ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_sprint_cancel ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_sprint_cancel, enabled ? JNI_TRUE : JNI_FALSE );
        }
    }

    void set_velocity ( const bool enabled ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_velocity ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_velocity, enabled ? JNI_TRUE : JNI_FALSE );
        }
    }

    void set_horizontal_multiplier ( const float value ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_horizontal_multiplier ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_horizontal_multiplier, static_cast < jdouble > ( value ) );
        }
    }

    void set_vertical_multiplier ( const float value ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_vertical_multiplier ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_vertical_multiplier, static_cast < jdouble > ( value ) );
        }
    }

    void set_no_slowness_fov ( const bool enabled ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_no_slowness_fov ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_no_slowness_fov, enabled ? JNI_TRUE : JNI_FALSE );
        }
    }

    void set_has_custom_time ( const bool enabled ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_has_custom_time ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_has_custom_time, enabled ? JNI_TRUE : JNI_FALSE );
        }
	}

    void set_custom_time ( const long long value ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_custom_time ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_custom_time, static_cast < jlong > ( value ) );
        }
    }

    void set_guard_check ( const bool enabled ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_guard_check ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_guard_check, enabled ? JNI_TRUE : JNI_FALSE );
        }
    }

    void set_autofishing ( const bool enabled ) const {
        const auto & cache = native_bridge_cache::get ( m_java );
        if ( cache.set_auto_fishing ) {
            m_java->get_env ( )->CallStaticVoidMethod ( cache.native_bridge_class, cache.set_auto_fishing, enabled ? JNI_TRUE : JNI_FALSE );
        }
    }
};