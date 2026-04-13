#pragma once
#include <algorithm>

#include "../java.h"

namespace mappings {
    inline void register_native_bridge_mappings ( mapping_set & set ) {
        set.classes [ xorstr_ ( "NativeBridge" ) ] = { xorstr_ ( "cosmicclient.NativeBridge" ), make_descriptor ( xorstr_ ( "cosmicclient.NativeBridge" ) ) };

        set.methods [ xorstr_ ( "NativeBridge.setInjected" ) ] = { xorstr_ ( "setInjected" ), xorstr_ ( "(Z)V" ), true };
        set.methods [ xorstr_ ( "NativeBridge.isInjected" ) ] = { xorstr_ ( "isInjected" ), xorstr_ ( "()Z" ), true };
        set.methods [ xorstr_ ( "NativeBridge.setClearLava" ) ] = { xorstr_ ( "setClearLava" ), xorstr_ ( "(Z)V" ), true };
        set.methods [ xorstr_ ( "NativeBridge.setHasReach" ) ] = { xorstr_ ( "setHasReach" ), xorstr_ ( "(Z)V" ), true };
        set.methods [ xorstr_ ( "NativeBridge.setReachValue" ) ] = { xorstr_ ( "setReachValue" ), xorstr_ ( "(F)V" ), true };
        set.methods [ xorstr_ ( "NativeBridge.setSprintCancel" ) ] = { xorstr_ ( "setSprintCancel" ), xorstr_ ( "(Z)V" ), true };
        set.methods [ xorstr_ ( "NativeBridge.setVelocity" ) ] = { xorstr_ ( "setVelocity" ), xorstr_ ( "(Z)V" ), true };
    	set.methods [ xorstr_ ( "NativeBridge.setHorizontalMultiplier" ) ] = { xorstr_ ( "setHorizontalMultiplier" ), xorstr_ ( "(D)V" ), true };
        set.methods [ xorstr_ ( "NativeBridge.setVerticalMultiplier" ) ] = { xorstr_ ( "setVerticalMultiplier" ), xorstr_ ( "(D)V" ), true };
        set.methods [ xorstr_ ( "NativeBridge.setNoSlownessFOV" ) ] = { xorstr_ ( "setNoSlownessFOV" ), xorstr_ ( "(Z)V" ), true };
        set.methods [ xorstr_ ( "NativeBridge.setHasCustomTime" ) ] = { xorstr_ ( "setHasCustomTime" ), xorstr_ ( "(Z)V" ), true };
        set.methods [ xorstr_ ( "NativeBridge.setCustomTime" ) ] = { xorstr_ ( "setCustomTime" ), xorstr_ ( "(J)V" ), true };
        set.methods [ xorstr_ ( "NativeBridge.setGuardCheck" ) ] = { xorstr_ ( "setGuardCheck" ), xorstr_ ( "(Z)V" ), true };
        set.methods [ xorstr_ ( "NativeBridge.setAutoFishing" ) ] = { xorstr_ ( "setAutoFishing" ), xorstr_ ( "(Z)V" ), true };

        set.fields [ xorstr_ ( "NativeBridge.injected" ) ] = { xorstr_ ( "injected" ), xorstr_ ( "Z" ), true };
        set.fields [ xorstr_ ( "NativeBridge.clearLava" ) ] = { xorstr_ ( "clearLava" ), xorstr_ ( "Z" ), true };
    }
}