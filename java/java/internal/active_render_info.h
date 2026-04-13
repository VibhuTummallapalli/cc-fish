#pragma once
#include "../../math/matrix.h"
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_active_render_info : public c_java_object {
private:
    matrix4<float> get_matrix ( const jobject object ) const {
        static jmethodID method = nullptr;
        if ( !method ) {
            const auto float_buffer_class = m_java->get_env ( )->FindClass ( xorstr_ ( "java/nio/FloatBuffer" ) );
            method = m_java->get_env (  )->GetMethodID ( float_buffer_class, xorstr_ ( "get" ), xorstr_ ( "(I)F" ) );
            m_java->get_env ( )->DeleteLocalRef ( float_buffer_class );
        }

        return matrix4<float>::from_getter ( [ & ] ( const int i ) {
            return m_java->get_env ( )->CallFloatMethod ( object, method, i );
        } );
    }

public:
    c_active_render_info ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] matrix4<float> get_projection ( ) const {
        const auto & cache = active_render_info_cache::get ( m_java );

        if ( !cache.projection )
			return { };

        return get_matrix ( m_java->get_env ( )->GetStaticObjectField ( cache.active_render_info_class, cache.projection ) );
    }

    [[nodiscard]] matrix4<float> get_model_view ( ) const {
        const auto & cache = active_render_info_cache::get ( m_java );

        if ( !cache.modelview )
            return { };

        return get_matrix ( m_java->get_env ( )->GetStaticObjectField ( cache.active_render_info_class, cache.modelview ) );
    }
};