// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_AttributeMenu.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "AbilitySystem/SSAttributeSet.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "AbilitySystem/Attributes/LyraCombatSet.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "Player/SSPlayerState.h"

void UMVVM_AttributeMenu::Initialize(ASSPlayerState* InSSPlayerState)
{
	if(!InSSPlayerState) return;
	SSPlayerState=InSSPlayerState;
	SSPlayerState->OnStatPointsChangedDelegate.AddDynamic(this, &UMVVM_AttributeMenu::OnStatPointsChanged);
}

void UMVVM_AttributeMenu::OnStatPointsChanged(int32 InStatPoints)
{
	StatPoints+=InStatPoints;
	SetStatPoints(InStatPoints);
}


void UMVVM_AttributeMenu::SetStatPoints(int32 InStatPoints)
{
	UE_MVVM_SET_PROPERTY_VALUE(StatPoints, InStatPoints);
}

void UMVVM_AttributeMenu::SetDamageStat(int32 InStat)
{
	UE_MVVM_SET_PROPERTY_VALUE(DamageStat, InStat);
}


void UMVVM_AttributeMenu::SetHealthStat(int32 InStat)
{
	UE_MVVM_SET_PROPERTY_VALUE(HealthStat, InStat);

}

void UMVVM_AttributeMenu::SetSpeedStat(int32 InStat)
{
	UE_MVVM_SET_PROPERTY_VALUE(SpeedStat, InStat);
}

void UMVVM_AttributeMenu::UpgradeStat(FString AttributeName)
{
	if (SSPlayerState->GetStatPoints()==0.f) return;
	SSPlayerState->AddToStatPoints(-1);

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	
	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SSPlayerState);
	
	if (AttributeName=="Damage")
	{
		ModifierInfo.Attribute=ULyraCombatSet::GetBaseDamageAttribute();
		ModifierInfo.ModifierMagnitude=FScalableFloat(2.0f);
		SetDamageStat(DamageStat+1);
	}
	else if (AttributeName=="Health")
	{
		ModifierInfo.Attribute=ULyraHealthSet::GetMaxHealthAttribute();
		ModifierInfo.ModifierMagnitude=FScalableFloat(50.f);
		SetHealthStat(HealthStat+1);
	}
	else if (AttributeName=="Speed")
	{
		ModifierInfo.Attribute=ULyraHealthSet::GetMoveSpeedAttribute();
		ModifierInfo.ModifierMagnitude=FScalableFloat(50.0f);
		SetSpeedStat(SpeedStat+1);
	}
	
	UGameplayEffect* Effect= NewObject<UGameplayEffect>( GetTransientPackage(), FName(TEXT("AttributeSet")));
	Effect->Modifiers.Add(ModifierInfo);
	
	FGameplayEffectContextHandle EffectContext=ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);;
	FGameplayEffectSpec* Spec= new FGameplayEffectSpec(Effect, EffectContext, 1.0f);
	ASC->ApplyGameplayEffectSpecToSelf(*Spec);
	
}
