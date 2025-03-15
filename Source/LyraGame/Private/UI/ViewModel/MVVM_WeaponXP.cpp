// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_WeaponXP.h"
#include"AbilitySystem/Data/LevelUpInfo.h"
#include "Public/Player/SSPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Tests/AutomationCommon.h"

void UMVVM_WeaponXP::Initialize(ASSPlayerState* InPlayerState)
{
	if(!InPlayerState) return;
	SSPlayerState=InPlayerState;
	SSPlayerState->OnWeaponXPChangedDelegate.AddDynamic(this, &UMVVM_WeaponXP::OnWeaponXPChanged);
	SSPlayerState->OnWeaponLevelChangedDelegate.AddDynamic(this, &UMVVM_WeaponXP::OnWeaponLevelChanged);
	SetWeaponLevel(1);
}

void UMVVM_WeaponXP::OnWeaponXPChanged(int32 NewXP)
{
	const ULevelUpInfo* WeaponLevelUpInfo=SSPlayerState->WeaponLevelUpInfo;
	
	const int32 Level=WeaponLevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel=WeaponLevelUpInfo->LevelUpInformation.Num();

	if(Level<=MaxLevel && Level>0)
	{
		const int32 LevelUpRequirement=WeaponLevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelRequirement=WeaponLevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

		const int32 DeltaLevelRequirement=LevelUpRequirement-PreviousLevelRequirement;
		const int32 XPForThisLevel=NewXP-PreviousLevelRequirement;

		const float XPBarPercent=static_cast<float>(XPForThisLevel)/static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UMVVM_WeaponXP::OnWeaponLevelChanged(int32 NewLevel)
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

