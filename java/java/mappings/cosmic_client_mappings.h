#pragma once
#include <algorithm>

#include "../java.h"

namespace mappings {
    inline void register_cosmic_client_mappings ( mapping_set & set ) {
        set.name = xorstr_ ( "CosmicClient" );
        set.test_class = xorstr_ ( "Minecraft" );

        // Classes
        set.classes [ xorstr_ ( "Minecraft" ) ] = { xorstr_ ( "cosmicclient.wb" ), make_descriptor ( xorstr_ ( "cosmicclient.wb" ) ) };
        set.classes [ xorstr_ ( "EntityPlayerSP" ) ] = { xorstr_ ( "cosmicclient.O7" ), make_descriptor ( xorstr_ ( "cosmicclient.O7" ) ) };
        set.classes [ xorstr_ ( "EntityPlayer" ) ] = { xorstr_ ( "cosmicclient.OM" ), make_descriptor ( xorstr_ ( "cosmicclient.OM" ) ) };
        set.classes [ xorstr_ ( "Entity" ) ] = { xorstr_ ( "cosmicclient.Me" ), make_descriptor ( xorstr_ ( "cosmicclient.Me" ) ) };
        set.classes [ xorstr_ ( "EntityLivingBase" ) ] = { xorstr_ ( "cosmicclient.N3" ), make_descriptor ( xorstr_ ( "cosmicclient.N3" ) ) };
        set.classes [ xorstr_ ( "EntityOtherPlayerMP" ) ] = { xorstr_ ( "cosmicclient.O1" ), make_descriptor ( xorstr_ ( "cosmicclient.O1" ) ) };
        set.classes [ xorstr_ ( "EntityArmorStand" ) ] = { xorstr_ ( "cosmicclient.Or" ), make_descriptor ( xorstr_ ( "cosmicclient.Or" ) ) };
        set.classes [ xorstr_ ( "EntityItem" ) ] = { xorstr_ ( "cosmicclient.Ot" ), make_descriptor ( xorstr_ ( "cosmicclient.Ot" ) ) };
        set.classes [ xorstr_ ( "EntityThrowable" ) ] = { xorstr_ ( "cosmicclient.Oc" ), make_descriptor ( xorstr_ ( "cosmicclient.Oc" ) ) };
        set.classes [ xorstr_ ( "EntityEnderPearl" ) ] = { xorstr_ ( "cosmicclient.Oy" ), make_descriptor ( xorstr_ ( "cosmicclient.Oy" ) ) };
        set.classes [ xorstr_ ( "World" ) ] = { xorstr_ ( "cosmicclient.YU" ), make_descriptor ( xorstr_ ( "cosmicclient.YU" ) ) };
        set.classes [ xorstr_ ( "WorldClient" ) ] = { xorstr_ ( "cosmicclient.YC" ), make_descriptor ( xorstr_ ( "cosmicclient.YC" ) ) };
        set.classes [ xorstr_ ( "ItemStack" ) ] = { xorstr_ ( "cosmicclient.ia" ), make_descriptor ( xorstr_ ( "cosmicclient.ia" ) ) };
        set.classes [ xorstr_ ( "Item" ) ] = { xorstr_ ( "cosmicclient.g0" ), make_descriptor ( xorstr_ ( "cosmicclient.g0" ) ) };
        set.classes [ xorstr_ ( "InventoryPlayer" ) ] = { xorstr_ ( "cosmicclient.KW" ), make_descriptor ( xorstr_ ( "cosmicclient.KW" ) ) };
        set.classes [ xorstr_ ( "Container" ) ] = { xorstr_ ( "cosmicclient.aPy" ), make_descriptor ( xorstr_ ( "cosmicclient.aPy" ) ) };
        set.classes [ xorstr_ ( "PlayerControllerMP" ) ] = { xorstr_ ( "cosmicclient.HH" ), make_descriptor ( xorstr_ ( "cosmicclient.HH" ) ) };
        set.classes [ xorstr_ ( "GameSettings" ) ] = { xorstr_ ( "cosmicclient.jy" ), make_descriptor ( xorstr_ ( "cosmicclient.jy" ) ) };
        set.classes [ xorstr_ ( "EntityRenderer" ) ] = { xorstr_ ( "cosmicclient.RS" ), make_descriptor ( xorstr_ ( "cosmicclient.RS" ) ) };
        set.classes [ xorstr_ ( "RenderManager" ) ] = { xorstr_ ( "cosmicclient.a2" ), make_descriptor ( xorstr_ ( "cosmicclient.a2" ) ) };
        set.classes [ xorstr_ ( "Timer" ) ] = { xorstr_ ( "cosmicclient.Kc" ), make_descriptor ( xorstr_ ( "cosmicclient.Kc" ) ) };
        set.classes [ xorstr_ ( "Vec3" ) ] = { xorstr_ ( "cosmicclient.qG" ), make_descriptor ( xorstr_ ( "cosmicclient.qG" ) ) };
        set.classes [ xorstr_ ( "AxisAlignedBB" ) ] = { xorstr_ ( "cosmicclient.IT" ), make_descriptor ( xorstr_ ( "cosmicclient.IT" ) ) };
        set.classes [ xorstr_ ( "MovingObjectPosition" ) ] = { xorstr_ ( "cosmicclient.UE" ), make_descriptor ( xorstr_ ( "cosmicclient.UE" ) ) };
        set.classes [ xorstr_ ( "MovingObjectType" ) ] = { xorstr_ ( "cosmicclient.rO" ), make_descriptor ( xorstr_ ( "cosmicclient.rO" ) ) };
        set.classes [ xorstr_ ( "PlayerCapabilities" ) ] = { xorstr_ ( "cosmicclient.aqE" ), make_descriptor ( xorstr_ ( "cosmicclient.aqE" ) ) };
        set.classes [ xorstr_ ( "PotionEffect" ) ] = { xorstr_ ( "cosmicclient.Q" ), make_descriptor ( xorstr_ ( "cosmicclient.Q" ) ) };
        set.classes [ xorstr_ ( "NBTTagCompound" ) ] = { xorstr_ ( "cosmicclient.Cp" ), make_descriptor ( xorstr_ ( "cosmicclient.Cp" ) ) };
        set.classes [ xorstr_ ( "CosmicClientSkins" ) ] = { xorstr_ ( "cosmicclient.gV" ), make_descriptor ( xorstr_ ( "cosmicclient.gV" ) ) };
        set.classes [ xorstr_ ( "CosmicClientSet" ) ] = { xorstr_ ( "cosmicclient.uL" ), make_descriptor ( xorstr_ ( "cosmicclient.uL" ) ) };
        set.classes [ xorstr_ ( "ActiveRenderInfo" ) ] = { xorstr_ ( "cosmicclient.dE" ), make_descriptor ( xorstr_ ( "cosmicclient.dE" ) ) };
        set.classes [ xorstr_ ( "CosmicPing" ) ] = { xorstr_ ( "cosmicclient.du" ), make_descriptor ( xorstr_ ( "cosmicclient.du" ) ) };
        set.classes [ xorstr_ ( "CosmicPacket" ) ] = { xorstr_ ( "cosmicclient.df" ), make_descriptor ( xorstr_ ( "cosmicclient.df" ) ) };
        set.classes [ xorstr_ ( "CosmicPacketHandler" ) ] = { xorstr_ ( "cosmicclient.IX" ), make_descriptor ( xorstr_ ( "cosmicclient.IX" ) ) };
        set.classes [ xorstr_ ( "CosmicClientHandler" ) ] = { xorstr_ ( "cosmicclient.ug" ), make_descriptor ( xorstr_ ( "cosmicclient.ug" ) ) };
        set.classes [ xorstr_ ( "EntityPotion") ] = { xorstr_ ( "cosmicclient.OR" ), make_descriptor ( xorstr_ ( "cosmicclient.OR" ) ) };
		set.classes [ xorstr_ ( "MouseHelper" ) ] = { xorstr_ ( "cosmicclient.tK" ), make_descriptor ( xorstr_ ( "cosmicclient.tK" ) ) };
        set.classes [ xorstr_ ( "AbstractClientPlayer" ) ] = { xorstr_ ( "cosmicclient.O4" ), make_descriptor ( xorstr_ ( "cosmicclient.O4" ) ) };
        set.classes [ xorstr_ ( "ResourceLocation" ) ] = { xorstr_ ( "cosmicclient.Dg" ), make_descriptor ( xorstr_ ( "cosmicclient.Dg" ) ) };
		set.classes [ xorstr_ ( "TextureManager" ) ] = { xorstr_ ( "cosmicclient.H6" ), make_descriptor ( xorstr_ ( "cosmicclient.H6" ) ) };
        set.classes [ xorstr_ ( "TextureObject" ) ] = { xorstr_ ( "cosmicclient.aHG" ), make_descriptor ( xorstr_ ( "cosmicclient.aHG" ) ) };
        set.classes [ xorstr_ ( "AbstractTexture" ) ] = { xorstr_ ( "cosmicclient.YB" ), make_descriptor ( xorstr_ ( "cosmicclient.YB" ) ) };
        set.classes [ xorstr_ ( "BlockPos" ) ] = { xorstr_ ( "cosmicclient.vn" ), make_descriptor ( xorstr_ ( "cosmicclient.vn" ) ) };
        set.classes [ xorstr_ ( "BlockState" ) ] = { xorstr_ ( "cosmicclient._t" ), make_descriptor ( xorstr_ ( "cosmicclient._t" ) ) };
        set.classes [ xorstr_ ( "Block" ) ] = { xorstr_ ( "cosmicclient.mz" ), make_descriptor ( xorstr_ ( "cosmicclient.mz" ) ) };
        set.classes [ xorstr_ ( "EntityFishHook" ) ] = { xorstr_ ( "cosmicclient.OV" ), make_descriptor ( xorstr_ ( "cosmicclient.OV" ) ) };
        set.classes [ xorstr_ ( "GuiInventory" ) ] = { xorstr_ ( "cosmicclient.F_" ), make_descriptor ( xorstr_ ( "cosmicclient.F_" ) ) };

        // EntityFishHook methods/fields
        set.fields [ xorstr_ ( "EntityFishHook.angler" ) ] = { xorstr_ ( "a" ), xorstr_ ( "{EntityPlayer}" ), false };
        set.classes [ xorstr_ ( "GuiScreen" ) ] = { xorstr_ ( "cosmicclient.Ev" ), make_descriptor ( xorstr_ ( "cosmicclient.Ev" ) ) };

        // Methods
        set.methods [ xorstr_ ( "Minecraft.getMinecraft" ) ] = { xorstr_ ( "ap" ), xorstr_ ( "(){Minecraft}" ), true };
        set.methods [ xorstr_ ( "Minecraft.getRenderManager" ) ] = { xorstr_ ( "E" ), xorstr_ ( "(){RenderManager}" ), false };
        set.methods [ xorstr_ ( "Minecraft.getTextureManager") ] = { xorstr_ ( "as" ), xorstr_ ( "(){TextureManager}" ), false };
        set.methods [ xorstr_ ( "Minecraft.sendClick" ) ] = { xorstr_ ( "b" ), xorstr_ ( "(IZCI)V" ), false };
        set.methods [ xorstr_ ( "Minecraft.displayGuiScreen" ) ] = { xorstr_ ( "b" ), xorstr_ ( "({GuiScreen}ICC)V" ), false };

        set.methods [ xorstr_ ( "Entity.getName" ) ] = { xorstr_ ( "b" ), xorstr_ ( "(JC)Ljava/lang/String;" ), false };
        set.methods [ xorstr_ ( "Entity.setPosition" ) ] = { xorstr_ ( "i" ), xorstr_ ( "(DDD)V" ), false };
        set.methods [ xorstr_ ( "Entity.isInvisible" ) ] = { xorstr_ ( "U" ), xorstr_ ( "()Z" ), false };
        set.methods [ xorstr_ ( "Entity.isEntityAlive" ) ] = { xorstr_ ( "k" ), xorstr_ ( "()Z" ), false };
        set.methods [ xorstr_ ( "Entity.getPositionEyes" ) ] = { xorstr_ ( "f" ), xorstr_ ( "(F){Vec3}" ), false };
        set.methods [ xorstr_ ( "Entity.uuid" ) ] = { xorstr_ ( "an" ), xorstr_ ( "()Ljava/util/UUID;" ), false };
        set.methods [ xorstr_ ( "Entity.setSprinting" ) ] = { xorstr_ ( "h" ), xorstr_ ( "(IZCS)V" ), false };

        set.methods [ xorstr_ ( "EntityLivingBase.getHealth" ) ] = { xorstr_ ( "a" ), xorstr_ ( "()F" ), false };
        set.methods [ xorstr_ ( "EntityLivingBase.getMaxHealth" ) ] = { xorstr_ ( "b" ), xorstr_ ( "()F" ), false };
        set.methods [ xorstr_ ( "EntityLivingBase.removeActivePotionEffect" ) ] = { xorstr_ ( "g" ), xorstr_ ( "(I)V" ), false };
        set.methods [ xorstr_ ( "EntityLivingBase.isPotionActive" ) ] = { xorstr_ ( "m" ), xorstr_ ( "(I)Z" ), false };
        set.methods [ xorstr_ ( "EntityLivingBase.getHeldItem" ) ] = { xorstr_ ( "v" ), xorstr_ ( "(){ItemStack}" ), false };
        set.methods [ xorstr_ ( "EntityLivingBase.getEquipmentInSlot" ) ] = { xorstr_ ( "h" ), xorstr_ ( "(I){ItemStack}" ), false };

        set.methods [ xorstr_ ( "EntityPlayer.doubleJump" ) ] = { xorstr_ ( "C" ), xorstr_ ( "()V" ), false };
        set.methods [ xorstr_ ( "EntityPlayer.getHeldItem" ) ] = { xorstr_ ( "v" ), xorstr_ ( "(){ItemStack}" ), false };

        set.methods [ xorstr_ ( "EntityItem.getItemStack" ) ] = { xorstr_ ( "p" ), xorstr_ ( "(){ItemStack}" ), false };

        set.methods [ xorstr_ ( "EntityThrowable.getGravityVelocity" ) ] = { xorstr_ ( "c" ), xorstr_ ( "()F" ), false };
        set.methods [ xorstr_ ( "EntityThrowable.getInaccuracy" ) ] = { xorstr_ ( "d" ), xorstr_ ( "()F" ), false };

        set.methods [ xorstr_ ( "World.rayTrace" ) ] = { xorstr_ ( "a" ), xorstr_ ( "({Vec3}{Vec3}SCZZIZ){MovingObjectPosition}" ), false };
        set.methods [ xorstr_ ( "World.getWorldTime" ) ] = { xorstr_ ( "v" ), xorstr_ ( "()J" ), false };
        set.methods [ xorstr_ ( "World.getTotalWorldTime" ) ] = { xorstr_ ( "C" ), xorstr_ ( "()J" ), false };
        set.methods [ xorstr_ ( "World.getBlockState" ) ] = { xorstr_ ( "b" ), xorstr_ ( "(J{BlockPos}B){BlockState}" ), false };

        set.methods [ xorstr_ ( "ItemStack.getName" ) ] = { xorstr_ ( "i" ), xorstr_ ( "(ISC)Ljava/lang/String;" ), false };
        set.methods [ xorstr_ ( "ItemStack.getFormattedName" ) ] = { xorstr_ ( "o" ), xorstr_ ( "(ICI)Ljava/lang/String;" ), false };
        set.methods [ xorstr_ ( "ItemStack.getSkin" ) ] = { xorstr_ ( "C" ), xorstr_ ( "(CIS){CosmicClientSkins}" ), false };
        set.methods [ xorstr_ ( "ItemStack.setTagCompound" ) ] = { xorstr_ ( "c" ), xorstr_ ( "({NBTTagCompound})V" ), false };
        set.methods [ xorstr_ ( "ItemStack.getItemDamage" ) ] = { xorstr_ ( "t" ), xorstr_ ( "()I" ), false };

        set.methods [ xorstr_ ( "Item.getIdFromItem" ) ] = { xorstr_ ( "a" ), xorstr_ ( "({Item})I" ), true };

        set.methods [ xorstr_ ( "InventoryPlayer.getItemStack" ) ] = { xorstr_ ( "b" ), xorstr_ ( "(I){ItemStack}" ), false };
        set.methods [ xorstr_ ( "InventoryPlayer.setInventorySlotContents" ) ] = { xorstr_ ( "b" ), xorstr_ ( "(JIB{ItemStack})V" ), false };
        set.methods [ xorstr_ ( "InventoryPlayer.removeStackFromSlot" ) ] = { xorstr_ ( "c" ), xorstr_ ( "(I){ItemStack}" ), false };
        set.methods [ xorstr_ ( "InventoryPlayer.setItemStack" ) ] = { xorstr_ ( "e" ), xorstr_ ( "({ItemStack})V" ), false };
        set.methods [ xorstr_ ( "InventoryPlayer.getSizeInventory" ) ] = { xorstr_ ( "e" ), xorstr_ ( "()I" ), false };

        set.methods [ xorstr_ ( "Container.putStackInSlot" ) ] = { xorstr_ ( "a" ), xorstr_ ( "(IICS{ItemStack})V" ), false };
        set.methods [ xorstr_ ( "Container.detectAndSendChanges" ) ] = { xorstr_ ( "c" ), xorstr_ ( "(ISC)V" ), false };
        set.methods [ xorstr_ ( "Container.transferStackInSlot" ) ] = { xorstr_ ( "a" ), xorstr_ ( "(IIISSI{EntityPlayer}){ItemStack}" ), false };

        set.methods [ xorstr_ ( "PlayerControllerMP.windowClick" ) ] = { xorstr_ ( "a" ), xorstr_ ( "(IIII{EntityPlayer}){ItemStack}" ), false };
        set.methods [ xorstr_ ( "PlayerControllerMP.sendUseItem" ) ] = { xorstr_ ( "a" ), xorstr_ ( "({EntityPlayer}{World}{ItemStack})Z" ), false };
        set.methods [ xorstr_ ( "PlayerControllerMP.attackEntity" ) ] = { xorstr_ ( "a" ), xorstr_ ( "(SSI{EntityPlayer}{Entity})V" ), false };

        set.methods [ xorstr_ ( "Vec3.init" ) ] = { xorstr_ ( "<init>" ), xorstr_ ( "(DDD)V" ), false };

        set.methods [ xorstr_ ( "NBTTagCompound.setString" ) ] = { xorstr_ ( "a" ), xorstr_ ( "(IISLjava/lang/String;Ljava/lang/String;)V" ), false };
        set.methods [ xorstr_ ( "NBTTagCompound.toString" ) ] = { xorstr_ ( "toString" ), xorstr_ ( "()Ljava/lang/String;" ), false };
        set.methods [ xorstr_ ( "NBTTagCompound.getCompoundTag" ) ] = { xorstr_ ( "n" ), xorstr_ ( "(ICSLjava/lang/String;){NBTTagCompound}" ), false };

        set.methods [ xorstr_ ( "PotionEffect.getName" ) ] = { xorstr_ ( "g" ), xorstr_ ( "()Ljava/lang/String;" ), false };

        set.methods [ xorstr_ ( "CosmicClientSkins.getName" ) ] = { xorstr_ ( "name" ), xorstr_ ( "()Ljava/lang/String;" ), false };

        set.methods [ xorstr_ ( "CosmicClientSet.getName" ) ] = { xorstr_ ( "name" ), xorstr_ ( "()Ljava/lang/String;" ), false };
        set.methods [ xorstr_ ( "CosmicClientSet.getId" ) ] = { xorstr_ ( "ordinal" ), xorstr_ ( "()I" ), false };
        set.methods [ xorstr_ ( "CosmicClientSet.getValues" ) ] = { xorstr_ ( "values" ), xorstr_ ( "()[Lcosmicclient/uL;" ), true };

        set.methods [ xorstr_ ( "MovingObjectType.getId" ) ] = { xorstr_ ( "ordinal" ), xorstr_ ( "()I" ), false };

        set.methods [ xorstr_ ( "CosmicPing.init" ) ] = { xorstr_ ( "<init>" ), xorstr_ ( "(DDDIZ)V" ), false };

        set.methods [ xorstr_ ( "CosmicPacketHandler.sendPacket" ) ] = { xorstr_ ( "b" ), xorstr_ ( "({CosmicPacket})V" ), false };

        set.methods [ xorstr_ ( "CosmicClientHandler.getPacketHandler" ) ] = { xorstr_ ( "F" ), xorstr_ ( "(){CosmicPacketHandler}" ), false };
        set.methods [ xorstr_ ( "CosmicClientHandler.getInstance" ) ] = { xorstr_ ( "i" ), xorstr_ ( "(){CosmicClientHandler}" ), true };

		set.methods [ xorstr_ ( "MouseHelper.grabMouseCursor" ) ] = { xorstr_ ( "c" ), xorstr_ ( "()V" ), false };
        set.methods [ xorstr_ ( "MouseHelper.mouseXYChange" ) ] = { xorstr_ ( "b" ), xorstr_ ( "()V" ), false };
        set.methods [ xorstr_ ( "MouseHelper.ungrabMouseCursor" ) ] = { xorstr_ ( "a" ), xorstr_ ( "()V" ), false };

		set.methods [ xorstr_ ( "AbstractClientPlayer.getLocationSkin" ) ] = { xorstr_ ( "k" ), xorstr_ ( "(){ResourceLocation}" ), false };
        set.methods [ xorstr_ ( "AbstractClientPlayer.getLocationCape" ) ] = { xorstr_ ( "e" ), xorstr_ ( "(){ResourceLocation}" ), false };

        set.methods [ xorstr_ ( "TextureManager.bindTexture" ) ] = { xorstr_ ( "a" ), xorstr_ ("({ResourceLocation})V" ), false };

        set.methods [ xorstr_ ( "TextureObject.getGlTextureId" ) ] = { xorstr_ ( "c" ), xorstr_ ( "()I" ), false };

        set.methods [ xorstr_ ( "ResourceLocation.init" ) ] = { xorstr_ ( "<init>" ), xorstr_ ( "(Ljava/lang/String;II)V" ), false };
        set.methods [ xorstr_ ( "ResourceLocation.toString" ) ] = { xorstr_ ( "toString" ), xorstr_ ( "()Ljava/lang/String;" ), false };

        set.methods [ xorstr_ ( "BlockPos.init" ) ] = { xorstr_ ( "<init>" ), xorstr_ ( "(DDD)V" ), false };

        set.methods [ xorstr_ ( "BlockState.getBlock" ) ] = { xorstr_ ( "b" ), xorstr_ ( "(){Block}" ), false };

        set.methods [ xorstr_ ( "Block.getIdFromBlock" ) ] = { xorstr_ ( "b" ), xorstr_ ( "({Block})I" ), true };

        set.methods [ xorstr_ ( "EntityRenderer.getMouseOver" ) ] = { xorstr_ ( "g" ), xorstr_ ( "(FCJ)V" ), false };

        set.methods [ xorstr_ ( "GuiInventory.init" ) ] = { xorstr_ ( "<init>" ), xorstr_ ( "({EntityPlayer})V" ), false };

        set.methods [ xorstr_ ( "EntityPlayerSP.swingItem" ) ] = { xorstr_ ( "M" ), xorstr_ ( "()V" ), false };

        // Fields
        set.fields [ xorstr_ ( "Minecraft.thePlayer" ) ] = { xorstr_ ( "D" ), xorstr_ ( "{EntityPlayerSP}" ), false };
        set.fields [ xorstr_ ( "Minecraft.theWorld" ) ] = { xorstr_ ( "H" ), xorstr_ ( "{WorldClient}" ), false };
        set.fields [ xorstr_ ( "Minecraft.rightClickDelay" ) ] = { xorstr_ ( "j" ), xorstr_ ( "I" ), false };
        set.fields [ xorstr_ ( "Minecraft.entityRenderer" ) ] = { xorstr_ ( "au" ), xorstr_ ( "{EntityRenderer}" ), false };
        set.fields [ xorstr_ ( "Minecraft.gameSettings" ) ] = { xorstr_ ( "aa" ), xorstr_ ( "{GameSettings}" ), false };
        set.fields [ xorstr_ ( "Minecraft.playerControllerMP" ) ] = { xorstr_ ( "Z" ), xorstr_ ( "{PlayerControllerMP}" ), false };
        set.fields [ xorstr_ ( "Minecraft.inGameHasFocus" ) ] = { xorstr_ ( "aC" ), xorstr_ ( "Z" ), false };
        set.fields [ xorstr_ ( "Minecraft.getTimer" ) ] = { xorstr_ ( "ar" ), xorstr_ ( "{Timer}" ), false };
		set.fields [ xorstr_ ( "Minecraft.getMouseHelper" ) ] = { xorstr_ ( "X" ), xorstr_ ( "{MouseHelper}" ), false };
        set.fields [ xorstr_ ( "Minecraft.objectMouseOver" ) ] = { xorstr_ ( "O" ), xorstr_ ( "{MovingObjectPosition}" ), false };
        set.fields [ xorstr_ ( "Minecraft.currentScreen" ) ] = { xorstr_ ( "ak" ), xorstr_ ( "{GuiScreen}" ), false };

        set.fields [ xorstr_ ( "Entity.posX" ) ] = { xorstr_ ( "t" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Entity.posY" ) ] = { xorstr_ ( "U" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Entity.posZ" ) ] = { xorstr_ ( "N" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Entity.lastPosX" ) ] = { xorstr_ ( "R" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Entity.lastPosY" ) ] = { xorstr_ ( "r" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Entity.lastPosZ" ) ] = { xorstr_ ( "aq" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Entity.motionX" ) ] = { xorstr_ ( "f" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Entity.motionY" ) ] = { xorstr_ ( "aa" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Entity.motionZ" ) ] = { xorstr_ ( "ak" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Entity.entityId" ) ] = { xorstr_ ( "ad" ), xorstr_ ( "I" ), false };
        set.fields [ xorstr_ ( "Entity.width" ) ] = { xorstr_ ( "O" ), xorstr_ ( "F" ), false };
        set.fields [ xorstr_ ( "Entity.boundingBox" ) ] = { xorstr_ ( "av" ), xorstr_ ( "{AxisAlignedBB}" ), false };
        set.fields [ xorstr_ ( "Entity.rotationPitch" ) ] = { xorstr_ ( "M" ), xorstr_ ( "F" ), false };
        set.fields [ xorstr_ ( "Entity.rotationYaw" ) ] = { xorstr_ ( "ae" ), xorstr_ ( "F" ), false };
        set.fields [ xorstr_ ( "Entity.prevRotationPitch" ) ] = { xorstr_ ( "u" ), xorstr_ ( "F" ), false };
        set.fields [ xorstr_ ( "Entity.prevRotationYaw" ) ] = { xorstr_ ( "V" ), xorstr_ ( "F" ), false };
        set.fields [ xorstr_ ( "Entity.isInWeb" ) ] = { xorstr_ ( "C" ), xorstr_ ( "Z" ), false };
        set.fields [ xorstr_ ( "Entity.ticksExisted" ) ] = { xorstr_ ( "x" ), xorstr_ ( "I" ), false };

        set.fields [ xorstr_ ( "EntityLivingBase.hurtTime" ) ] = { xorstr_ ( "aS" ), xorstr_ ( "I" ), false };
        set.fields [ xorstr_ ( "EntityLivingBase.activePotionMap" ) ] = { xorstr_ ( "aP" ), xorstr_ ( "Ljava/util/Map;" ), false };

        set.fields [ xorstr_ ( "EntityPlayer.getCapabilities" ) ] = { xorstr_ ( "bu" ), xorstr_ ( "{PlayerCapabilities}" ), false };
        set.fields [ xorstr_ ( "EntityPlayer.getInventory" ) ] = { xorstr_ ( "bM" ), xorstr_ ( "{InventoryPlayer}" ), false };
        set.fields [ xorstr_ ( "EntityPlayer.getContainer" ) ] = { xorstr_ ( "bS" ), xorstr_ ( "{Container}" ), false };
    	set.fields [ xorstr_ ( "EntityPlayer.itemInUse" ) ] = { xorstr_ ( "bG" ), xorstr_ ( "{ItemStack}" ), false };
        set.fields [ xorstr_ ( "EntityPlayer.itemInUseCount" ) ] = { xorstr_ ( "b0" ), xorstr_ ( "I" ), false };

        set.fields [ xorstr_ ( "EntityThrowable.thrower" ) ] = { xorstr_ ( "aB" ), xorstr_ ( "{EntityLivingBase}" ), false };

        set.fields [ xorstr_ ( "World.playerEntities" ) ] = { xorstr_ ( "l" ), xorstr_ ( "Ljava/util/List;" ), false };
        set.fields [ xorstr_ ( "World.cloudColor" ) ] = { xorstr_ ( "r" ), xorstr_ ( "J" ), false };

        set.fields [ xorstr_ ( "WorldClient.entityList" ) ] = { xorstr_ ( "V" ), xorstr_ ( "Ljava/util/Set;" ), false };

        set.fields [ xorstr_ ( "ItemStack.item" ) ] = { xorstr_ ( "o" ), xorstr_ ( "{Item}" ), false };
        set.fields [ xorstr_ ( "ItemStack.set" ) ] = { xorstr_ ( "t" ), xorstr_ ( "{CosmicClientSet}" ), false };
        set.fields [ xorstr_ ( "ItemStack.stackSize" ) ] = { xorstr_ ( "m" ), xorstr_ ( "I" ), false };
        set.fields [ xorstr_ ( "ItemStack.getNBTCompound" ) ] = { xorstr_ ( "n" ), xorstr_ ( "{NBTTagCompound}" ), false };

        set.fields [ xorstr_ ( "InventoryPlayer.getActiveSlot" ) ] = { xorstr_ ( "d" ), xorstr_ ( "I" ), false };
        set.fields [ xorstr_ ( "InventoryPlayer.getCurrentItem" ) ] = { xorstr_ ( "b" ), xorstr_ ( "{ItemStack}" ), false };

        set.fields [ xorstr_ ( "GameSettings.getFov" ) ] = { xorstr_ ( "B" ), xorstr_ ( "F" ), false };

        set.fields [ xorstr_ ( "Entityrenderer.thirdpersonDistance" ) ] = { xorstr_ ( "b" ), xorstr_ ( "F" ), false };

        set.fields [ xorstr_ ( "RenderManager.renderPosX" ) ] = { xorstr_ ( "b" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "RenderManager.renderPosY" ) ] = { xorstr_ ( "v" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "RenderManager.renderPosZ" ) ] = { xorstr_ ( "d" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "RenderManager.viewerPosX" ) ] = { xorstr_ ( "h" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "RenderManager.viewerPosY" ) ] = { xorstr_ ( "c" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "RenderManager.viewerPosZ" ) ] = { xorstr_ ( "r" ), xorstr_ ( "D" ), false };

        set.fields [ xorstr_ ( "Timer.renderPartialTicks" ) ] = { xorstr_ ( "b" ), xorstr_ ( "F" ), false };

        set.fields [ xorstr_ ( "Vec3.xCoord" ) ] = { xorstr_ ( "a" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Vec3.yCoord" ) ] = { xorstr_ ( "b" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Vec3.zCoord" ) ] = { xorstr_ ( "c" ), xorstr_ ( "D" ), false };

        set.fields [ xorstr_ ( "AxisAlignedBB.minX" ) ] = { xorstr_ ( "a" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "AxisAlignedBB.minY" ) ] = { xorstr_ ( "c" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "AxisAlignedBB.minZ" ) ] = { xorstr_ ( "d" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "AxisAlignedBB.maxX" ) ] = { xorstr_ ( "f" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "AxisAlignedBB.maxY" ) ] = { xorstr_ ( "b" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "AxisAlignedBB.maxZ" ) ] = { xorstr_ ( "e" ), xorstr_ ( "D" ), false };

        set.fields [ xorstr_ ( "MovingObjectPosition.entityHit" ) ] = { xorstr_ ( "c" ), xorstr_ ( "{Entity}" ), false };
        set.fields [ xorstr_ ( "MovingObjectPosition.hitVec" ) ] = { xorstr_ ( "a" ), xorstr_ ( "{Vec3}" ), false };
        set.fields [ xorstr_ ( "MovingObjectPosition.typeOfHit" ) ] = { xorstr_ ( "d" ), xorstr_ ( "{MovingObjectType}" ), false };

        set.fields [ xorstr_ ( "PlayerCapabilities.allowEdit" ) ] = { xorstr_ ( "a" ), xorstr_ ( "Z" ), false };
        set.fields [ xorstr_ ( "PlayerCapabilities.allowFlying" ) ] = { xorstr_ ( "d" ), xorstr_ ( "Z" ), false };
        set.fields [ xorstr_ ( "PlayerCapabilities.isCreativeMode" ) ] = { xorstr_ ( "e" ), xorstr_ ( "Z" ), false };
        set.fields [ xorstr_ ( "PlayerCapabilities.isFlying" ) ] = { xorstr_ ( "h" ), xorstr_ ( "Z" ), false };
        set.fields [ xorstr_ ( "PlayerCapabilities.walkSpeed" ) ] = { xorstr_ ( "f" ), xorstr_ ( "F" ), false };

        set.fields [ xorstr_ ( "PotionEffect.duration" ) ] = { xorstr_ ( "c" ), xorstr_ ( "I" ), false };
        set.fields [ xorstr_ ( "PotionEffect.id" ) ] = { xorstr_ ( "j" ), xorstr_ ( "I" ), false };

        set.fields [ xorstr_ ( "ActiveRenderInfo.PROJECTION" ) ] = { xorstr_ ( "b" ), xorstr_ ( "Ljava/nio/FloatBuffer;" ), true };
        set.fields [ xorstr_ ( "ActiveRenderInfo.MODELVIEW" ) ] = { xorstr_ ( "d" ), xorstr_ ( "Ljava/nio/FloatBuffer;" ), true };

        set.fields [ xorstr_ ( "ResourceLocation.resourceDomain" ) ] = { xorstr_ ( "resourceDomain" ), xorstr_ ( "Ljava/lang/String;" ), false };
        set.fields [ xorstr_ ( "ResourceLocation.resourcePath" ) ] = { xorstr_ ( "resourcePath" ), xorstr_ ( "Ljava/lang/String;" ), false };

		set.fields [ xorstr_ ( "TextureManager.mapTextureObjects" ) ] = { xorstr_ ( "c" ), xorstr_ ( "Ljava/util/Map;" ), false };
		set.fields [ xorstr_ ( "TextureManager.mapTextureCounters" ) ] = { xorstr_ ( "a" ), xorstr_ ( "Ljava/util/Map;" ), false };

        set.fields [ xorstr_ ( "AbstractTexture.glTextureId" ) ] = { xorstr_ ( "c" ), xorstr_ ( "I" ), false };

        set.fields [ xorstr_ ( "Block.maxX" ) ] = { xorstr_ ( "b" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Block.maxY" ) ] = { xorstr_ ( "F" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Block.maxZ" ) ] = { xorstr_ ( "L" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Block.minX" ) ] = { xorstr_ ( "D" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Block.minY" ) ] = { xorstr_ ( "B" ), xorstr_ ( "D" ), false };
        set.fields [ xorstr_ ( "Block.minZ" ) ] = { xorstr_ ( "P" ), xorstr_ ( "D" ), false };
    }
}