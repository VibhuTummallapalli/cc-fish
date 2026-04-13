#pragma once
#include "cache/cached.h"
#include "java_object/java_object.h"

class c_mc_vector : public c_java_object {
public:
    c_mc_vector ( const jobject obj, c_java * java, const bool owns_ref = true )
        : c_java_object ( obj, java, owns_ref ) {
    }

    [[nodiscard]] double get_x() const {
        const auto & cache = vec3_cache::get ( m_java );
        return cache.x_coord ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.x_coord ) : 0.0;
    }
    [[nodiscard]] double get_y ( ) const {
        const auto & cache = vec3_cache::get ( m_java );
        return cache.y_coord ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.y_coord ) : 0.0;
    }
    [[nodiscard]] double get_z ( ) const {
        const auto & cache = vec3_cache::get ( m_java );
        return cache.z_coord ? m_java->get_env ( )->GetDoubleField ( m_obj, cache.z_coord ) : 0.0;
    }
    [[nodiscard]] vec3 < double > get_vec3 ( ) const {
        return { get_x ( ), get_y ( ), get_z ( ) };
    }

    c_mc_vector from_vector ( const vec3<double> & in ) {
        const auto & cache = vec3_cache::get ( m_java );
        const jobject init = cache.init ? m_java->get_env ( )->NewObject ( cache.vector_class, cache.init, in.x, in.y, in.z ) : nullptr;
        return { init, m_java, true };
    }
};