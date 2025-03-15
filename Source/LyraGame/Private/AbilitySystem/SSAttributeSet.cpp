// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SSAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/LyraGameplayAbilityTargetData_SingleTargetHit.h"
#include "Character/SSCharacter.h"
#include "Interaction/CombatInterface.h"
#include "Weapons/SSWeaponBase.h"

USSAttributeSet::USSAttributeSet()
	:RifleXP(0)
	,ShotgunXP(0)
	,PistolXP(0)
{
	
}

void USSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute==GetPistolXPAttribute())
	{
		HandleIncomingXP(GetPistolXP(),EWeaponType::Pistol,Data);
		SetPistolXP(0.f);
	}
	if( Data.EvaluatedData.Attribute==GetRifleXPAttribute())
	{
		HandleIncomingXP(GetRifleXP(),EWeaponType::Rifle,Data);
		SetRifleXP(0.f);
	}
	if( Data.EvaluatedData.Attribute==GetShotgunXPAttribute())
	{
		HandleIncomingXP(GetShotgunXP(),EWeaponType::Shotgun,Data);
		SetShotgunXP(0.f);
	}
	if( Data.EvaluatedData.Attribute==GetIncomingXPAttribute())
	{
		HandleIncomingPlayerXP(GetIncomingXP(),Data);
		SetIncomingXP(0.f);
	}
	
}

void USSAttributeSet::HandleIncomingPlayerXP(int32 XP, const FGameplayEffectModCallbackData& Data) 
{
	const float LocalIncomingXP=XP;
	
	ASSCharacter* SourceCharacter=Cast<ASSCharacter>(Data.Target.GetAvatarActor());

	if(SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel=ICombatInterface::Execute_GetPlayerLevel(SourceCharacter);
		const int32 CurrentXP=ICombatInterface::Execute_GetPlayerXP(SourceCharacter);

		const int32 NewLevel=ICombatInterface::Execute_FindPlayerLevelForXP(SourceCharacter, CurrentXP+LocalIncomingXP);
		const int32 NumLevelUps=NewLevel-CurrentLevel;
		if(NumLevelUps>0)
		{
			const int32 StatPointsReward=ICombatInterface::Execute_GetStatPointsReward(SourceCharacter,CurrentLevel);
			
			ICombatInterface::Execute_AddPlayerLevel(SourceCharacter, NumLevelUps);
			ICombatInterface::Execute_AddStatPoints(SourceCharacter, StatPointsReward);
		}
		ICombatInterface::Execute_AddPlayerXP(SourceCharacter, LocalIncomingXP);
	}
}

void USSAttributeSet::HandleIncomingXP(int32 XP, EWeaponType WeaponType, const FGameplayEffectModCallbackData& Data)
{

	ASSCharacter* AvatarActor=Cast<ASSCharacter>(Data.Target.GetAvatarActor());
	
	const int32 CurrentLevel=ICombatInterface::Execute_GetWeaponLevel(AvatarActor, WeaponType);
	const int32 CurrentXP=ICombatInterface::Execute_GetWeaponXP(AvatarActor, WeaponType);

	const int32 NewLevel=ICombatInterface::Execute_FindWeaponLevelForXP(AvatarActor, CurrentXP+XP);
	const int32 NumLevelUps=NewLevel-CurrentLevel;

	if(NumLevelUps>0)
	{
		ICombatInterface::Execute_AddWeaponLevel(AvatarActor,NumLevelUps,WeaponType);
		AvatarActor->OnLevelUp(WeaponType,NewLevel);
	}
	ICombatInterface::Execute_AddWeaponXP(AvatarActor,XP,WeaponType);
}
