// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_WeaponXP.h"
#include"AbilitySystem/Data/LevelUpInfo.h"
#include "Character/SSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Tests/AutomationCommon.h"

void UMVVM_WeaponXP::Initialize(ASSCharacter* InCharacter)
{
	if(!InCharacter) return;
	Character=InCharacter;
	Character->OnXPChangedDelegate.AddDynamic(this, &UMVVM_WeaponXP::OnXPChanged);
	Character->OnLevelChangedDelegate.AddDynamic(this, &UMVVM_WeaponXP::OnLevelChanged);
	SetWeaponLevel(1);
}

void UMVVM_WeaponXP::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo=Character->LevelUpInfo;
	
	const int32 Level=LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel=LevelUpInfo->LevelUpInformation.Num();

	if(Level<=MaxLevel && Level>0)
	{
		const int32 LevelUpRequirement=LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelRequirement=LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

		const int32 DeltaLevelRequirement=LevelUpRequirement-PreviousLevelRequirement;
		const int32 XPForThisLevel=NewXP-PreviousLevelRequirement;

		const float XPBarPercent=static_cast<float>(XPForThisLevel)/static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UMVVM_WeaponXP::OnLevelChanged(int32 NewLevel)
{
	SetWeaponLevel(NewLevel);
}

void UMVVM_WeaponXP::SetWeaponXP(int32 InXP)
{
	UE_MVVM_SET_PROPERTY_VALUE(WeaponXP, InXP);
}

void UMVVM_WeaponXP::SetWeaponLevel(int32 InLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(WeaponLevel, InLevel);
}

