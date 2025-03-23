// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SSCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AsyncTreeDifferences.h"
#include "GameplayEffect.h"
#include "LyraGameplayTags.h"
#include "AbilitySystem/SSAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/LyraAnimInstance.h"
#include "Animation/SSAnimInstance.h"
#include "Camera/LyraCameraComponent.h"
#include "Character/LyraCharacterMovementComponent.h"
#include "Character/LyraHealthComponent.h"
#include "Player/SSPlayerState.h"
#include "UI/ViewModel/MVVM_WeaponXP.h"
#include "Weapons/LyraWeaponSpawner.h"
#include "Weapons/SSWeaponBase.h"




ASSCharacter::ASSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<ULyraCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetMesh()->CastShadow = true;
	GetMesh()->bCastHiddenShadow = true;
	
	Mesh1p= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1p"));
	Mesh1p->SetupAttachment(CameraComponent);
	Mesh1p->SetOnlyOwnerSee(true);
	Mesh1p->CastShadow = false;
	Mesh1p->SetOwnerNoSee(true);
	
}

float ASSCharacter::GetSpeed() const
{
	return HealthComponent ? HealthComponent->GetMoveSpeed() : 0.0f;
}

void ASSCharacter::SetCurrentWeapon(ASSWeaponBase* NewWeapon)
{
	CurrentWeapon= NewWeapon;;
}

void ASSCharacter::OnPlayerEquippedNewWeapon(float WeaponOffset)
{
	if(!IsValid(GetMesh())) return;
	
	if (USSAnimInstance* AnimInstance = Cast<USSAnimInstance>(Mesh1p->GetAnimInstance()))
	{
		AnimInstance->OnNewWeaponEquipped(WeaponOffset);
	}
}

void ASSCharacter::SetIsFirstPerson()
{
	if(Mesh1p->bOwnerNoSee==true)
	{
		Mesh1p->SetOwnerNoSee(false);
		IsFirstPerson=true;
	}
	else
	{
		Mesh1p->SetOwnerNoSee(true);
		IsFirstPerson=false;
	}
}

void ASSCharacter::OnLevelUp(EWeaponType WeaponType, int32 Level)
{
	for (const auto& Row : WeaponItemTable->GetRowMap())
	{
		FWeaponItem* WeaponData = (FWeaponItem*)Row.Value;
		if (WeaponData->WeaponType ==  WeaponType && WeaponData->WeaponLevel == Level)
		{
			WeaponItemDefinition = WeaponData->WeaponItemClass;
			WeaponLevelUp(WeaponItemDefinition, this, WeaponType);
			break;
		}
	}
}

FVector ASSCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	
	if(MontageTag.MatchesTagExact(LyraGameplayTags::CombatSocket_Weapon)&IsValid(CurrentWeapon))
	{
		return CurrentWeapon->GetWeaponMesh3P()->GetSocketLocation(WeaponTipSocketName);
	}
	if(MontageTag.MatchesTagExact(LyraGameplayTags::CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
		
	}
	if(MontageTag.MatchesTagExact(LyraGameplayTags::CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}

	return FVector();
}

int32 ASSCharacter::GetWeaponLevel_Implementation(EWeaponType WeaponType) const
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	return SSPlayerState->GetWeaponLevel(WeaponType);
}

int32 ASSCharacter::GetWeaponXP_Implementation(EWeaponType WeaponType) const
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	return SSPlayerState->GetWeaponXP(WeaponType);
}


int32 ASSCharacter::FindWeaponLevelForXP_Implementation(int32 InXP) const
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	return SSPlayerState->WeaponLevelUpInfo->FindLevelForXP(InXP);
}
void ASSCharacter::AddWeaponXP_Implementation(int32 InXP,EWeaponType WeaponType) 
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	SSPlayerState->AddToWeaponXP(InXP,WeaponType);
}

void ASSCharacter::AddWeaponLevel_Implementation(int32 InLevel, EWeaponType WeaponType)
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	SSPlayerState->AddToWeaponLevel(InLevel,WeaponType);
}

int32 ASSCharacter::GetPlayerLevel_Implementation() const
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	return SSPlayerState->GetPlayerLevel();
}

int32 ASSCharacter::GetPlayerXP_Implementation() const
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	return SSPlayerState->GetPlayerXP();
}

void ASSCharacter::AddPlayerXP_Implementation(int32 InXP)
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	SSPlayerState->AddToPlayerXP(InXP);
}

void ASSCharacter::AddPlayerLevel_Implementation(int32 InLevel)
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	SSPlayerState->AddToPlayerLevel(InLevel);
}

int32 ASSCharacter::FindPlayerLevelForXP_Implementation(int32 InXP) const
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	return SSPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

void ASSCharacter::AddStatPoints_Implementation(int32 InStatPoints)
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	SSPlayerState->AddToStatPoints(InStatPoints);
}

int32 ASSCharacter::GetStatPoints_Implementation() const
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	return SSPlayerState->GetStatPoints();
}

int32 ASSCharacter::GetStatPointsReward_Implementation(int32 Level) const
{
	ASSPlayerState* SSPlayerState=GetPlayerState<ASSPlayerState>();
	check(SSPlayerState);
	return SSPlayerState->LevelUpInfo->LevelUpInformation[Level].StatPointAward;
}


void ASSCharacter::SendWeaponXPAttribute(EWeaponType WeaponType)
{
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.ModifierMagnitude = FScalableFloat(100.0f);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	if(WeaponType==EWeaponType::Pistol)
	{
		ModifierInfo.Attribute =USSAttributeSet::GetPistolXPAttribute();
	}
	if(WeaponType==EWeaponType::Rifle)
	{
		ModifierInfo.Attribute =USSAttributeSet::GetRifleXPAttribute();
	}
	if(WeaponType==EWeaponType::Shotgun)
	{
		ModifierInfo.Attribute =USSAttributeSet::GetShotgunXPAttribute();
	}
	UGameplayEffect* Effect= NewObject<UGameplayEffect>( GetTransientPackage(), FName(TEXT("WeaponXP")));
	Effect->Modifiers.Add(ModifierInfo);
	
	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);

	FGameplayEffectContextHandle EffectContext=ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);;
	FGameplayEffectSpec* Spec= new FGameplayEffectSpec(Effect, EffectContext, 1.0f);
	ASC->ApplyGameplayEffectSpecToSelf(*Spec);
}
