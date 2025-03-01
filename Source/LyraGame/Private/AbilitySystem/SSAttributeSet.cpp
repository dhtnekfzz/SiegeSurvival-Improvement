// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SSAttributeSet.h"
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
}

void USSAttributeSet::HandleIncomingXP(int32 XP, EWeaponType WeaponType, const FGameplayEffectModCallbackData& Data)
{

	ASSCharacter* AvatarActor=Cast<ASSCharacter>(Data.Target.GetAvatarActor());
	
	const int32 CurrentLevel=ICombatInterface::Execute_GetLevel(AvatarActor, WeaponType);
	const int32 CurrentXP=ICombatInterface::Execute_GetXP(AvatarActor, WeaponType);

	const int32 NewLevel=ICombatInterface::Execute_FindLevelForXP(AvatarActor, CurrentXP+XP);
	const int32 NumLevelUps=NewLevel-CurrentLevel;

	if(NumLevelUps>0)
	{
		ICombatInterface::Execute_AddWeaponLevel(AvatarActor,NumLevelUps,WeaponType);
	}
	ICombatInterface::Execute_AddXP(AvatarActor,XP,WeaponType);
}
