// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_PlayerXP.h"
#include "Player/SSPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

void UMVVM_PlayerXP::Initialize(ASSPlayerState* InSSPlayerState)
{
	if(!InSSPlayerState) return;
	SSPlayerState=InSSPlayerState;
	SSPlayerState->OnPlayerXPChangedDelegate.AddDynamic(this, &UMVVM_PlayerXP::OnPlayerXPChanged);
	SSPlayerState->OnPlayerLevelChangedDelegate.AddDynamic(this, &UMVVM_PlayerXP::OnPlayerLevelChanged);
	SetPlayerLevel(1);
}

void UMVVM_PlayerXP::SetPlayerXP(int32 InXP)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerXP, InXP);
}

void UMVVM_PlayerXP::SetPlayerLevel(int32 InLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, InLevel);
}

void UMVVM_PlayerXP::OnPlayerXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo=SSPlayerState->LevelUpInfo;
	
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

void UMVVM_PlayerXP::OnPlayerLevelChanged(int32 NewLevel)
{
 	SetPlayerLevel(NewLevel);
}

