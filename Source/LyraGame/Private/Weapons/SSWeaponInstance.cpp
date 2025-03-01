// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SSWeaponInstance.h"

USSWeaponInstance::USSWeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

TSubclassOf<UAnimInstance> USSWeaponInstance::BestPickAnimLayer(bool bEquipped,
	const FGameplayTagContainer& CosmeticTags, TSubclassOf<UAnimInstance>& AnimLayer_1P) const
{
	const FLyraAnimLayerSelectionSet& SetToQuery_3P = (bEquipped ? EquippedAnimSet_3P : UnequippedAnimSet_3P);
	const FLyraAnimLayerSelectionSet& SetToQuery_1P = (bEquipped ? EquippedAnimSet_1P : UnequippedAnimSet_1P);
	AnimLayer_1P = SetToQuery_1P.SelectBestLayer(CosmeticTags);
	return SetToQuery_3P.SelectBestLayer(CosmeticTags);
}
