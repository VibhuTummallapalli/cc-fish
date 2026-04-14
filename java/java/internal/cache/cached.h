#pragma once
#include "../../java/java.h"

// Cached field/method IDs for NativeBridge
struct native_bridge_cache {
    jclass native_bridge_class = nullptr;
    jmethodID set_injected = nullptr;
    jmethodID is_injected = nullptr;
    jmethodID set_clear_lava = nullptr;
    jmethodID set_has_reach = nullptr;
	jmethodID set_reach_value = nullptr;
    jmethodID set_sprint_cancel = nullptr;
    jmethodID set_velocity = nullptr;
	jmethodID set_horizontal_multiplier = nullptr;
	jmethodID set_vertical_multiplier = nullptr;
    jmethodID set_no_slowness_fov = nullptr;
    jmethodID set_has_custom_time = nullptr;
    jmethodID set_custom_time = nullptr;
    jmethodID set_guard_check = nullptr;
    jmethodID set_auto_fishing = nullptr;
    jfieldID injected_field = nullptr;
    jfieldID clear_lava_field = nullptr;

    static native_bridge_cache & get ( c_java * java ) {
        static native_bridge_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.native_bridge_class = java->get_class ( xorstr_ ( "NativeBridge" ) );
            cache.set_injected = java->get_method ( xorstr_ ( "NativeBridge.setInjected" ) );
            cache.is_injected = java->get_method ( xorstr_ ( "NativeBridge.isInjected" ) );
            cache.set_clear_lava = java->get_method ( xorstr_ ( "NativeBridge.setClearLava" ) );
			cache.set_has_reach = java->get_method ( xorstr_ ( "NativeBridge.setHasReach" ) );
			cache.set_reach_value = java->get_method ( xorstr_ ( "NativeBridge.setReachValue" ) );
            cache.set_sprint_cancel = java->get_method ( xorstr_ ( "NativeBridge.setSprintCancel" ) );
			cache.set_velocity = java->get_method ( xorstr_ ( "NativeBridge.setVelocity" ) );
			cache.set_horizontal_multiplier = java->get_method ( xorstr_ ( "NativeBridge.setHorizontalMultiplier" ) );
			cache.set_vertical_multiplier = java->get_method ( xorstr_ ( "NativeBridge.setVerticalMultiplier" ) );
            cache.set_no_slowness_fov = java->get_method ( xorstr_ ( "NativeBridge.setNoSlownessFOV" ) );
			cache.set_has_custom_time = java->get_method ( xorstr_ ( "NativeBridge.setHasCustomTime" ) );
			cache.set_custom_time = java->get_method ( xorstr_ ( "NativeBridge.setCustomTime" ) );
			cache.set_guard_check = java->get_method ( xorstr_ ( "NativeBridge.setGuardCheck" ) );
            cache.set_auto_fishing = java->get_method ( xorstr_ ( "NativeBridge.setAutoFishing" ) );
            cache.injected_field = java->get_field ( xorstr_ ( "NativeBridge.injected" ) );
            cache.clear_lava_field = java->get_field ( xorstr_ ( "NativeBridge.clearLava" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for c_entity
struct entity_cache {
    jclass entity_living_base_class = nullptr;
    jfieldID pos_x = nullptr;
    jfieldID pos_y = nullptr;
    jfieldID pos_z = nullptr;
    jfieldID last_pos_x = nullptr;
    jfieldID last_pos_y = nullptr;
    jfieldID last_pos_z = nullptr;
    jfieldID motion_x = nullptr;
    jfieldID motion_y = nullptr;
    jfieldID motion_z = nullptr;
    jfieldID rotation_yaw = nullptr;
    jfieldID rotation_pitch = nullptr;
    jfieldID prev_rotation_yaw = nullptr;
    jfieldID prev_rotation_pitch = nullptr;
    jfieldID entity_id = nullptr;
    jfieldID width = nullptr;
    jfieldID ticks_existed = nullptr;
    jfieldID is_in_web = nullptr;
    jfieldID hurt_time = nullptr;
    jfieldID bounding_box = nullptr;
    jfieldID active_potion_map = nullptr;
    jmethodID get_name = nullptr;
    jmethodID is_invisible = nullptr;
    jmethodID is_entity_alive = nullptr;
    jmethodID set_position = nullptr;
    jmethodID uuid = nullptr;
    jmethodID set_sprinting = nullptr;
    jmethodID get_health = nullptr;
    jmethodID get_max_health = nullptr;
    jmethodID get_held_item = nullptr;
    jmethodID get_equipment_in_slot = nullptr;
    jmethodID remove_active_potion_effect = nullptr;
    jmethodID is_potion_active = nullptr;
    jmethodID get_position_eyes = nullptr;
    jmethodID swing_item = nullptr;

    static entity_cache & get ( c_java * java ) {
        static entity_cache cache;
        static bool initialized = false;

        if ( !initialized ) {
			cache.entity_living_base_class = java->get_class ( xorstr_ ( "EntityLivingBase" ) );
            cache.pos_x = java->get_field ( xorstr_ ( "Entity.posX" ) );
            cache.pos_y = java->get_field ( xorstr_ ( "Entity.posY" ) );
            cache.pos_z = java->get_field ( xorstr_ ( "Entity.posZ" ) );
            cache.last_pos_x = java->get_field ( xorstr_ ( "Entity.lastPosX" ) );
            cache.last_pos_y = java->get_field ( xorstr_ ( "Entity.lastPosY" ) );
            cache.last_pos_z = java->get_field ( xorstr_ ( "Entity.lastPosZ" ) );
            cache.motion_x = java->get_field ( xorstr_ ( "Entity.motionX" ) );
            cache.motion_y = java->get_field ( xorstr_ ( "Entity.motionY" ) );
            cache.motion_z = java->get_field ( xorstr_ ( "Entity.motionZ" ) );
            cache.rotation_yaw = java->get_field ( xorstr_ ( "Entity.rotationYaw" ) );
            cache.rotation_pitch = java->get_field ( xorstr_ ( "Entity.rotationPitch" ) );
            cache.prev_rotation_yaw = java->get_field ( xorstr_ ( "Entity.prevRotationYaw" ) );
            cache.prev_rotation_pitch = java->get_field ( xorstr_ ( "Entity.prevRotationPitch" ) );
            cache.entity_id = java->get_field ( xorstr_ ( "Entity.entityId" ) );
            cache.width = java->get_field ( xorstr_ ( "Entity.width" ) );
            cache.ticks_existed = java->get_field ( xorstr_ ( "Entity.ticksExisted" ) );
            cache.is_in_web = java->get_field ( xorstr_ ( "Entity.isInWeb" ) );
            cache.hurt_time = java->get_field ( xorstr_ ( "EntityLivingBase.hurtTime" ) );
            cache.bounding_box = java->get_field ( xorstr_ ( "Entity.boundingBox" ) );
            cache.active_potion_map = java->get_field ( xorstr_ ( "EntityLivingBase.activePotionMap" ) );
            cache.get_name = java->get_method ( xorstr_ ( "Entity.getName" ) );
            cache.is_invisible = java->get_method ( xorstr_ ( "Entity.isInvisible" ) );
            cache.is_entity_alive = java->get_method ( xorstr_ ( "Entity.isEntityAlive" ) );
            cache.set_position = java->get_method ( xorstr_ ( "Entity.setPosition" ) );
            cache.uuid = java->get_method ( xorstr_ ( "Entity.uuid" ) );
            cache.set_sprinting = java->get_method ( xorstr_ ( "Entity.setSprinting" ) );
            cache.get_health = java->get_method ( xorstr_ ( "EntityLivingBase.getHealth" ) );
            cache.get_max_health = java->get_method ( xorstr_ ( "EntityLivingBase.getMaxHealth" ) );
            cache.get_held_item = java->get_method ( xorstr_ ( "EntityLivingBase.getHeldItem" ) );
            cache.get_equipment_in_slot = java->get_method ( xorstr_ ( "EntityLivingBase.getEquipmentInSlot" ) );
            cache.remove_active_potion_effect = java->get_method ( xorstr_ ( "EntityLivingBase.removeActivePotionEffect" ) );
            cache.is_potion_active = java->get_method ( xorstr_ ( "EntityLivingBase.isPotionActive" ) );
            cache.get_position_eyes = java->get_method ( xorstr_ ( "Entity.getPositionEyes" ) );
			cache.swing_item = java->get_method ( xorstr_ ( "EntityPlayerSP.swingItem" ) );
            initialized = true;
        }

        return cache;
    }
};

// Cached field/method IDs for c_entity_player
struct entity_player_cache {
    jclass entity_player_class = nullptr;
    jfieldID capabilities = nullptr;
    jfieldID inventory = nullptr;
    jfieldID container = nullptr;
    jfieldID item_in_use = nullptr;
	jfieldID item_in_use_count = nullptr;
    jmethodID double_jump = nullptr;
    jmethodID get_held_item = nullptr;

    static entity_player_cache & get ( c_java * java ) {
        static entity_player_cache cache;
        static bool initialized = false;

        if ( !initialized ) {
			cache.entity_player_class = java->get_class ( xorstr_ ( "EntityPlayer" ) );
            cache.capabilities = java->get_field ( xorstr_ ( "EntityPlayer.getCapabilities" ) );
            cache.inventory = java->get_field ( xorstr_ ( "EntityPlayer.getInventory" ) );
            cache.container = java->get_field ( xorstr_ ( "EntityPlayer.getContainer" ) );
			cache.item_in_use = java->get_field ( xorstr_ ( "EntityPlayer.itemInUse" ) );
			cache.item_in_use_count = java->get_field ( xorstr_ ( "EntityPlayer.itemInUseCount" ) );
            cache.double_jump = java->get_method ( xorstr_ ( "EntityPlayer.doubleJump" ) );
            cache.get_held_item = java->get_method ( xorstr_ ( "EntityPlayer.getHeldItem" ) );
            initialized = true;
        }

        return cache;
    }
};

// Cached field/method IDs for c_entity_item
struct entity_item_cache {
    jmethodID get_item_stack = nullptr;

    static entity_item_cache & get ( c_java * java ) {
        static entity_item_cache cache;
        static bool initialized = false;

        if ( !initialized ) {
            cache.get_item_stack = java->get_method ( xorstr_ ( "EntityItem.getItemStack" ) );
            initialized = true;
        }

        return cache;
    }
};

// Cached field/method IDs for c_entity_throwable
struct entity_throwable_cache {
    jfieldID thrower = nullptr;
    jmethodID get_gravity_velocity = nullptr;
    jmethodID get_inaccuracy = nullptr;

    static entity_throwable_cache & get ( c_java * java ) {
        static entity_throwable_cache cache;
        static bool initialized = false;

        if ( !initialized ) {
            cache.thrower = java->get_field ( xorstr_ ( "EntityThrowable.thrower" ) );
            cache.get_gravity_velocity = java->get_method ( xorstr_ ( "EntityThrowable.getGravityVelocity" ) );
            cache.get_inaccuracy = java->get_method ( xorstr_ ( "EntityThrowable.getInaccuracy" ) );
            initialized = true;
        }

        return cache;
    }
};

// Cached field/method IDs for c_itemstack
struct itemstack_cache {
    jfieldID item = nullptr;
    jfieldID set = nullptr;
    jfieldID stack_size = nullptr;
    jfieldID get_nbt_compound = nullptr;
    jmethodID get_name = nullptr;
    jmethodID get_formatted_name = nullptr;
    jmethodID get_skin = nullptr;
    jmethodID set_tag_compound = nullptr;
    jmethodID get_item_damage = nullptr;

    static itemstack_cache & get ( c_java * java ) {
        static itemstack_cache cache;
        static bool initialized = false;

        if ( !initialized ) {
            cache.item = java->get_field ( xorstr_ ( "ItemStack.item" ) );
            cache.set = java->get_field ( xorstr_ ( "ItemStack.set" ) );
            cache.stack_size = java->get_field ( xorstr_ ( "ItemStack.stackSize" ) );
            cache.get_nbt_compound = java->get_field ( xorstr_ ( "ItemStack.getNBTCompound" ) );
            cache.get_name = java->get_method ( xorstr_ ( "ItemStack.getName" ) );
            cache.get_formatted_name = java->get_method ( xorstr_ ( "ItemStack.getFormattedName" ) );
            cache.get_skin = java->get_method ( xorstr_ ( "ItemStack.getSkin" ) );
            cache.set_tag_compound = java->get_method ( xorstr_ ( "ItemStack.setTagCompound" ) );
            cache.get_item_damage = java->get_method ( xorstr_ ( "ItemStack.getItemDamage" ) );

            initialized = true;
        }

        return cache;
    }
};

// Cached field/method IDs for c_item
struct item_cache {
    jclass item_class = nullptr;
    jmethodID get_id_from_item = nullptr;

    static item_cache & get ( c_java * java ) {
        static item_cache cache;
        static bool initialized = false;

        if ( !initialized ) {
            cache.item_class = java->get_class ( xorstr_ ( "Item" ) );
            cache.get_id_from_item = java->get_method ( xorstr_ ( "Item.getIdFromItem" ) );

            initialized = true;
        }

        return cache;
    }
};

// Cached field/method IDs for c_inventory_player
struct inventory_player_cache {
    jfieldID active_slot = nullptr;
    jfieldID current_item = nullptr;
    jmethodID get_item_stack = nullptr;
    jmethodID set_inventory_slot_contents = nullptr;
    jmethodID remove_stack_from_slot = nullptr;
    jmethodID set_item_stack = nullptr;
    jmethodID get_inventory_size = nullptr;

    static inventory_player_cache & get ( c_java * java ) {
        static inventory_player_cache cache;
        static bool initialized = false;

        if ( !initialized ) {
            cache.active_slot = java->get_field ( xorstr_ ( "InventoryPlayer.getActiveSlot" ) );
            cache.current_item = java->get_field ( xorstr_ ( "InventoryPlayer.getCurrentItem" ) );
            cache.get_item_stack = java->get_method ( xorstr_ ( "InventoryPlayer.getItemStack" ) );
            cache.set_inventory_slot_contents = java->get_method ( xorstr_ ( "InventoryPlayer.setInventorySlotContents" ) );
            cache.remove_stack_from_slot = java->get_method ( xorstr_ ( "InventoryPlayer.removeStackFromSlot" ) );
            cache.set_item_stack = java->get_method ( xorstr_ ( "InventoryPlayer.setItemStack" ) );
			cache.get_inventory_size = java->get_method ( xorstr_ ( "InventoryPlayer.getSizeInventory" ) );
            initialized = true;
        }

        return cache;
    }
};

// Cached field/method IDs for c_container
struct container_cache {
    jmethodID put_stack_in_slot = nullptr;
    jmethodID detect_and_send_changes = nullptr;
    jmethodID transfer_stack_in_slot = nullptr;

    static container_cache & get ( c_java * java ) {
        static container_cache cache;
        static bool initialized = false;

        if ( !initialized ) {
            cache.put_stack_in_slot = java->get_method ( xorstr_ ( "Container.putStackInSlot" ) );
            cache.detect_and_send_changes = java->get_method ( xorstr_ ( "Container.detectAndSendChanges" ) );
            cache.transfer_stack_in_slot = java->get_method ( xorstr_ ( "Container.transferStackInSlot" ) );
            initialized = true;
        }

        return cache;
    }
};

// Cached field/method IDs for c_minecraft
struct minecraft_cache {
    jfieldID the_player = nullptr;
    jfieldID the_world = nullptr;
    jfieldID entity_renderer = nullptr;
    jfieldID game_settings = nullptr;
    jfieldID player_controller_mp = nullptr;
    jfieldID get_timer = nullptr;
    jfieldID in_game_has_focus = nullptr;
    jfieldID right_click_delay = nullptr;
    jfieldID mouse_helper = nullptr;
    jfieldID object_mouse_over = nullptr;
    jfieldID current_screen = nullptr;
    jmethodID get_render_manager = nullptr;
    jmethodID get_minecraft = nullptr;
    jmethodID get_texture_manager = nullptr;
    jmethodID send_click = nullptr;
    jmethodID display_gui_screen = nullptr;

    static minecraft_cache & get ( c_java * java ) {
        static minecraft_cache cache;
        static bool initialized = false;

        if ( !initialized ) {
            cache.the_player = java->get_field ( xorstr_ ( "Minecraft.thePlayer" ) );
            cache.the_world = java->get_field ( xorstr_ ( "Minecraft.theWorld" ) );
            cache.entity_renderer = java->get_field ( xorstr_ ( "Minecraft.entityRenderer" ) );
            cache.game_settings = java->get_field ( xorstr_ ( "Minecraft.gameSettings" ) );
            cache.player_controller_mp = java->get_field ( xorstr_ ( "Minecraft.playerControllerMP" ) );
            cache.get_timer = java->get_field ( xorstr_ ( "Minecraft.getTimer" ) );
            cache.in_game_has_focus = java->get_field ( xorstr_ ( "Minecraft.inGameHasFocus" ) );
            cache.right_click_delay = java->get_field ( xorstr_ ( "Minecraft.rightClickDelay" ) );
            cache.mouse_helper = java->get_field ( xorstr_ ( "Minecraft.getMouseHelper" ) );
			cache.object_mouse_over = java->get_field ( xorstr_ ( "Minecraft.objectMouseOver" ) );
            cache.current_screen = java->get_field ( xorstr_ ( "Minecraft.currentScreen" ) );
            cache.get_render_manager = java->get_method ( xorstr_ ( "Minecraft.getRenderManager" ) );
            cache.get_minecraft = java->get_method ( xorstr_ ( "Minecraft.getMinecraft" ) );
			cache.get_texture_manager = java->get_method ( xorstr_ ( "Minecraft.getTextureManager" ) );
            cache.send_click = java->get_method ( xorstr_ ( "Minecraft.sendClick" ) );
			cache.display_gui_screen = java->get_method ( xorstr_ ( "Minecraft.displayGuiScreen" ) );
            initialized = true;
        }

        return cache;
    }
};

// Cached field/method IDs for c_render_manager
struct render_manager_cache {
    jfieldID render_pos_x = nullptr;
    jfieldID render_pos_y = nullptr;
    jfieldID render_pos_z = nullptr;
    jfieldID viewer_pos_x = nullptr;
    jfieldID viewer_pos_y = nullptr;
    jfieldID viewer_pos_z = nullptr;

    static render_manager_cache & get ( c_java * java ) {
        static render_manager_cache cache;
        static bool initialized = false;

        if ( !initialized ) {
            cache.render_pos_x = java->get_field ( xorstr_ ( "RenderManager.renderPosX" ) );
            cache.render_pos_y = java->get_field ( xorstr_ ( "RenderManager.renderPosY" ) );
            cache.render_pos_z = java->get_field ( xorstr_ ( "RenderManager.renderPosZ" ) );
            cache.viewer_pos_x = java->get_field ( xorstr_ ( "RenderManager.viewerPosX" ) );
            cache.viewer_pos_y = java->get_field ( xorstr_ ( "RenderManager.viewerPosY" ) );
            cache.viewer_pos_z = java->get_field ( xorstr_ ( "RenderManager.viewerPosZ" ) );
            initialized = true;
        }

        return cache;
    }
};

// Cached field/method IDs for entity renderer
struct entity_renderer_cache {
    jmethodID get_mouse_over = nullptr;
    jfieldID thirdperson_distance = nullptr;

    static entity_renderer_cache & get ( c_java * java ) {
        static entity_renderer_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
			cache.get_mouse_over = java->get_method ( xorstr_ ( "EntityRenderer.getMouseOver" ) );
            cache.thirdperson_distance = java->get_field ( xorstr_ ( "Entityrenderer.thirdpersonDistance" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for game settings
struct game_settings_cache {
    jfieldID fov = nullptr;

    static game_settings_cache & get ( c_java * java ) {
        static game_settings_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.fov = java->get_field ( xorstr_ ( "GameSettings.getFov" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for player controller mp
struct player_controller_mp_cache {
    jmethodID window_click = nullptr;
    jmethodID send_use_item = nullptr;
    jmethodID attack_entity = nullptr;

    static player_controller_mp_cache & get ( c_java * java ) {
        static player_controller_mp_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.window_click = java->get_method ( xorstr_ ( "PlayerControllerMP.windowClick" ) );
            cache.send_use_item = java->get_method ( xorstr_ ( "PlayerControllerMP.sendUseItem" ) );
			cache.attack_entity = java->get_method ( xorstr_ ( "PlayerControllerMP.attackEntity" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for timer
struct timer_cache {
    jfieldID render_partial_ticks = nullptr;

    static timer_cache & get ( c_java * java ) {
        static timer_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.render_partial_ticks = java->get_field ( xorstr_ ( "Timer.renderPartialTicks" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for world
struct world_cache {
    jfieldID player_entities = nullptr;
    jfieldID entities = nullptr;
    jfieldID cloud_color = nullptr;
    jmethodID get_world_time = nullptr;
    jmethodID get_total_world_time = nullptr;
    jmethodID ray_trace = nullptr;
    jmethodID get_block_state = nullptr;

    static world_cache & get ( c_java * java ) {
        static world_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.player_entities = java->get_field ( xorstr_ ( "World.playerEntities" ) );
            cache.entities = java->get_field ( xorstr_ ( "WorldClient.entityList" ) );
            cache.cloud_color = java->get_field ( xorstr_ ( "World.cloudColor" ) );
            cache.get_world_time = java->get_method ( xorstr_ ( "World.getWorldTime" ) );
            cache.get_total_world_time = java->get_method ( xorstr_ ( "World.getTotalWorldTime" ) );
            cache.ray_trace = java->get_method ( xorstr_ ( "World.rayTrace" ) );
			cache.get_block_state = java->get_method ( xorstr_ ( "World.getBlockState" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for vec3
struct vec3_cache {
    jclass vector_class = nullptr;
    jfieldID x_coord = nullptr;
    jfieldID y_coord = nullptr;
    jfieldID z_coord = nullptr;
    jmethodID init = nullptr;

    static vec3_cache & get ( c_java * java ) {
        static vec3_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
			cache.vector_class = java->get_class ( xorstr_ ( "Vec3" ) );
            cache.x_coord = java->get_field ( xorstr_ ( "Vec3.xCoord" ) );
            cache.y_coord = java->get_field ( xorstr_ ( "Vec3.yCoord" ) );
            cache.z_coord = java->get_field ( xorstr_ ( "Vec3.zCoord" ) );
            cache.init = java->get_method ( xorstr_ ( "Vec3.init" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for axis aligned bb
struct axis_aligned_bb_cache {
    jfieldID min_x = nullptr;
    jfieldID min_y = nullptr;
    jfieldID min_z = nullptr;
    jfieldID max_x = nullptr;
    jfieldID max_y = nullptr;
    jfieldID max_z = nullptr;

    static axis_aligned_bb_cache & get ( c_java * java ) {
        static axis_aligned_bb_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.min_x = java->get_field ( xorstr_ ( "AxisAlignedBB.minX" ) );
            cache.min_y = java->get_field ( xorstr_ ( "AxisAlignedBB.minY" ) );
            cache.min_z = java->get_field ( xorstr_ ( "AxisAlignedBB.minZ" ) );
            cache.max_x = java->get_field ( xorstr_ ( "AxisAlignedBB.maxX" ) );
            cache.max_y = java->get_field ( xorstr_ ( "AxisAlignedBB.maxY" ) );
            cache.max_z = java->get_field ( xorstr_ ( "AxisAlignedBB.maxZ" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for moving object position
struct moving_object_position_cache {
    jfieldID entity_hit = nullptr;
    jfieldID hit_vec = nullptr;
    jfieldID type_of_hit = nullptr;

    static moving_object_position_cache & get ( c_java * java ) {
        static moving_object_position_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.entity_hit    = java->get_field ( xorstr_ ( "MovingObjectPosition.entityHit" ) );
            cache.hit_vec       = java->get_field ( xorstr_ ( "MovingObjectPosition.hitVec" ) );
            cache.type_of_hit   = java->get_field ( xorstr_ ( "MovingObjectPosition.typeOfHit" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for moving object type
struct moving_object_type_cache {
    jmethodID get_id = nullptr;

    static moving_object_type_cache & get ( c_java * java ) {
        static moving_object_type_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.get_id = java->get_method ( xorstr_ ( "MovingObjectType.getId" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for player capabilities
struct player_capabilities_cache {
    jfieldID allow_edit = nullptr;
    jfieldID allow_flying = nullptr;
    jfieldID is_creative_mode = nullptr;
    jfieldID is_flying = nullptr;
    jfieldID walk_speed = nullptr;

    static player_capabilities_cache & get ( c_java * java ) {
        static player_capabilities_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.allow_edit = java->get_field ( xorstr_ ( "PlayerCapabilities.allowEdit" ) );
            cache.allow_flying = java->get_field ( xorstr_ ( "PlayerCapabilities.allowFlying" ) );
            cache.is_creative_mode = java->get_field ( xorstr_ ( "PlayerCapabilities.isCreativeMode" ) );
            cache.is_flying = java->get_field ( xorstr_ ( "PlayerCapabilities.isFlying" ) );
            cache.walk_speed = java->get_field ( xorstr_ ( "PlayerCapabilities.walkSpeed" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for potion effect
struct potion_effect_cache {
    jfieldID duration = nullptr;
    jfieldID id = nullptr;
    jmethodID get_name = nullptr;

    static potion_effect_cache & get ( c_java * java ) {
        static potion_effect_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.duration = java->get_field ( xorstr_ ( "PotionEffect.duration" ) );
            cache.id = java->get_field ( xorstr_ ( "PotionEffect.id" ) );
            cache.get_name = java->get_method ( xorstr_ ( "PotionEffect.getName" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for nbt tag compound
struct nbt_tag_compound_cache {
    jmethodID set_string = nullptr;
    jmethodID to_string = nullptr;
    jmethodID get_compound_tag = nullptr;

    static nbt_tag_compound_cache & get ( c_java * java ) {
        static nbt_tag_compound_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.set_string = java->get_method ( xorstr_ ( "NBTTagCompound.setString" ) );
            cache.to_string = java->get_method ( xorstr_ ( "NBTTagCompound.toString" ) );
            cache.get_compound_tag = java->get_method ( xorstr_ ( "NBTTagCompound.getCompoundTag" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for cosmic client skins
struct cosmic_client_skins_cache {
    jmethodID get_name = nullptr;

    static cosmic_client_skins_cache & get ( c_java * java ) {
        static cosmic_client_skins_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.get_name = java->get_method ( xorstr_ ( "CosmicClientSkins.getName" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for cosmic client set
struct cosmic_client_set_cache {
    jmethodID get_name = nullptr;
    jmethodID get_id = nullptr;
    jmethodID get_values = nullptr;

    static cosmic_client_set_cache & get ( c_java * java ) {
        static cosmic_client_set_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.get_name = java->get_method ( xorstr_ ( "CosmicClientSet.getName" ) );
            cache.get_id = java->get_method ( xorstr_ ( "CosmicClientSet.getId" ) );
            cache.get_values = java->get_method ( xorstr_ ( "CosmicClientSet.getValues" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for active render info
struct active_render_info_cache {
    jclass active_render_info_class = nullptr;
    jfieldID projection = nullptr;
    jfieldID modelview = nullptr;

    static active_render_info_cache & get ( c_java * java ) {
        static active_render_info_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.active_render_info_class = java->get_class ( xorstr_ ( "ActiveRenderInfo" ) );
            cache.projection = java->get_field ( xorstr_ ( "ActiveRenderInfo.PROJECTION" ) );
            cache.modelview = java->get_field ( xorstr_ ( "ActiveRenderInfo.MODELVIEW" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for cosmic ping
struct cosmic_ping_cache {
	jclass cosmic_ping_class = nullptr;
    jmethodID init = nullptr;

    static cosmic_ping_cache & get ( c_java * java ) {
        static cosmic_ping_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
			cache.cosmic_ping_class = java->get_class ( xorstr_ ( "CosmicPing" ) );
            cache.init = java->get_method ( xorstr_ ( "CosmicPing.init" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for cosmic packet handler
struct cosmic_packet_handler_cache {
    jmethodID send_packet = nullptr;

    static cosmic_packet_handler_cache & get ( c_java * java ) {
        static cosmic_packet_handler_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.send_packet = java->get_method ( xorstr_ ( "CosmicPacketHandler.sendPacket" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for cosmic client handler
struct cosmic_client_handler_cache {
    jmethodID get_packet_handler = nullptr;
    jmethodID get_instance = nullptr;

    static cosmic_client_handler_cache & get ( c_java * java ) {
        static cosmic_client_handler_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.get_packet_handler = java->get_method ( xorstr_ ( "CosmicClientHandler.getPacketHandler" ) );
            cache.get_instance = java->get_method ( xorstr_ ( "CosmicClientHandler.getInstance" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for entity ender pearl
struct entity_ender_pearl_cache {
    jclass entity_ender_pearl_class = nullptr;

    static entity_ender_pearl_cache & get ( c_java * java ) {
        static entity_ender_pearl_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.entity_ender_pearl_class = java->get_class ( xorstr_ ( "EntityEnderPearl" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for entity potion
struct entity_potion_cache {
    jclass entity_potion_class = nullptr;

    static entity_potion_cache & get ( c_java * java ) {
        static entity_potion_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.entity_potion_class = java->get_class ( xorstr_ ( "EntityPotion" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for mouse helper
struct mouse_helper_cache {
    jmethodID grab_mouse_cursor = nullptr;
    jmethodID mouse_xy_change = nullptr;
	jmethodID ungrab_mouse_cursor = nullptr;

    static mouse_helper_cache & get ( c_java * java ) {
        static mouse_helper_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
			cache.grab_mouse_cursor = java->get_method ( xorstr_ ( "MouseHelper.grabMouseCursor" ) );
			cache.mouse_xy_change = java->get_method ( xorstr_ ( "MouseHelper.mouseXYChange" ) );
			cache.ungrab_mouse_cursor = java->get_method ( xorstr_ ( "MouseHelper.ungrabMouseCursor" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for abstract client player
struct abstract_client_player_cache {
    jmethodID get_location_skin = nullptr;
    jmethodID get_location_cape = nullptr;

    static abstract_client_player_cache & get ( c_java * java ) {
        static abstract_client_player_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.get_location_skin = java->get_method ( xorstr_ ( "AbstractClientPlayer.getLocationSkin" ) );
            cache.get_location_cape = java->get_method ( xorstr_ ( "AbstractClientPlayer.getLocationCape" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for resource location
struct resource_location_cache {
    jclass resource_location_class = nullptr;
    jfieldID resource_domain = nullptr;
    jfieldID resource_path = nullptr;
    jmethodID init = nullptr;
    jmethodID to_string = nullptr;

    static resource_location_cache & get ( c_java * java ) {
        static resource_location_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
			cache.resource_location_class = java->get_class ( xorstr_ ( "ResourceLocation" ) );
			cache.resource_domain = java->get_field ( xorstr_ ( "ResourceLocation.resourceDomain" ) );
			cache.resource_path = java->get_field ( xorstr_ ( "ResourceLocation.resourcePath" ) );
			cache.init = java->get_method ( xorstr_ ( "ResourceLocation.init" ) );
            cache.to_string = java->get_method ( xorstr_ ( "ResourceLocation.toString" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for texture manager
struct texture_manager_cache {
    jfieldID map_texture_objects = nullptr;
    jfieldID map_texture_counters = nullptr;
    jmethodID bind_texture = nullptr;

    static texture_manager_cache & get ( c_java * java ) {
        static texture_manager_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
			cache.map_texture_objects = java->get_field ( xorstr_ ( "TextureManager.mapTextureObjects" ) );
			cache.map_texture_counters = java->get_field ( xorstr_ ( "TextureManager.mapTextureCounters" ) );
			cache.bind_texture = java->get_method ( xorstr_ ( "TextureManager.bindTexture" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for texture object
struct texture_object_cache {
    jmethodID get_gl_texture_id = nullptr;

    static texture_object_cache & get ( c_java * java ) {
        static texture_object_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.get_gl_texture_id = java->get_method ( xorstr_ ( "TextureObject.getGlTextureId" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for texture object
struct abstract_texture_cache {
    jfieldID gl_texture_id = nullptr;

    static abstract_texture_cache & get ( c_java * java ) {
        static abstract_texture_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.gl_texture_id = java->get_field ( xorstr_ ( "AbstractTexture.glTextureId" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for block pos
struct block_pos_cache {
    jclass block_pos_class = nullptr;
    jmethodID init = nullptr;

    static block_pos_cache & get ( c_java * java ) {
        static block_pos_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
			cache.block_pos_class = java->get_class ( xorstr_ ( "BlockPos" ) );
            cache.init = java->get_method ( xorstr_ ( "BlockPos.init" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for block state
struct block_state_cache {
    jmethodID get_block = nullptr;

    static block_state_cache & get ( c_java * java ) {
        static block_state_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.get_block = java->get_method ( xorstr_ ( "BlockState.getBlock" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for block
struct block_cache {
    jclass block_class = nullptr;
    jmethodID get_id_from_block = nullptr;
    jfieldID max_x = nullptr;
    jfieldID max_y = nullptr;
    jfieldID max_z = nullptr;
    jfieldID min_x = nullptr;
    jfieldID min_y = nullptr;
    jfieldID min_z = nullptr;

    static block_cache & get ( c_java * java ) {
        static block_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.block_class = java->get_class ( xorstr_ ( "Block" ) );
            cache.get_id_from_block = java->get_method ( xorstr_ ( "Block.getIdFromBlock" ) );
			cache.max_x = java->get_field ( xorstr_ ( "Block.maxX" ) );
			cache.max_y = java->get_field ( xorstr_ ( "Block.maxY" ) );
			cache.max_z = java->get_field ( xorstr_ ( "Block.maxZ" ) );
			cache.min_x = java->get_field ( xorstr_ ( "Block.minX" ) );
			cache.min_y = java->get_field ( xorstr_ ( "Block.minY" ) );
			cache.min_z = java->get_field ( xorstr_ ( "Block.minZ" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for entity fish hook
struct entity_fish_hook_cache {
    jclass entity_fish_hook_class = nullptr;
    jfieldID angler = nullptr;

    static entity_fish_hook_cache & get ( c_java * java ) {
        static entity_fish_hook_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.entity_fish_hook_class = java->get_class ( xorstr_ ( "EntityFishHook" ) );
            cache.angler = java->get_field ( xorstr_ ( "EntityFishHook.angler" ) );
            initialized = true;
        }
        return cache;
    }
};

// Cached field/method IDs for gui inventory
struct gui_inventory_cache {
    jclass gui_inventory_class = nullptr;
	jmethodID init = nullptr;

    static gui_inventory_cache & get ( c_java * java ) {
        static gui_inventory_cache cache;
        static bool initialized = false;
        if ( !initialized ) {
            cache.gui_inventory_class = java->get_class ( xorstr_ ( "GuiInventory" ) );
			cache.init = java->get_method ( xorstr_ ( "GuiInventory.init" ) );
            initialized = true;
        }
        return cache;
    }
};