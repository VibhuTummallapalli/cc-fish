#pragma once
#include "modules/module_handler.h"
#include "modules/chams/chams.h"

class c_hack {
public:
    c_module_handler module_handler;
    c_chams chams;

    explicit c_hack ( c_java * java ) 
        : module_handler ( java )
        , chams ( java ) {
    }

    void initialize ( ) {
        module_handler.initialize ( );
    }
};

inline c_hack * g_hack = nullptr;
