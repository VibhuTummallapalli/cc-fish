#pragma once
#include "../../../java/java.h"

// Placeholder chams module - renders entities with custom colors
class c_chams {
private:
    c_java * m_java;
    bool m_enabled = false;

public:
    explicit c_chams ( c_java * java ) : m_java ( java ) { }

    void on_render_entity_start ( long long uuid_most, long long uuid_least ) {
        if ( !m_enabled ) return;
        // TODO: Implement entity render start hook (change OpenGL states)
    }

    void on_render_entity_end ( long long uuid_most, long long uuid_least ) {
        if ( !m_enabled ) return;
        // TODO: Implement entity render end hook (restore OpenGL states)
    }

    void set_enabled ( bool enabled ) { m_enabled = enabled; }
    [[nodiscard]] bool is_enabled ( ) const { return m_enabled; }
};
