#pragma once
#include "fishing/fishing_simple.h"

class c_module_handler {
private:
    c_java * m_java;
    c_fishing m_fishing;

public:
    explicit c_module_handler ( c_java * java ) 
        : m_java ( java )
        , m_fishing ( java ) {
    }

    void initialize ( ) {
        // Initialize all modules here
    }

    c_fishing & get_fishing ( ) {
        return m_fishing;
    }
};
