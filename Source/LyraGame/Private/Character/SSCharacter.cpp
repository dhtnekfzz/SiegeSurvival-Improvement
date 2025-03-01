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

	FWeaponExperience NewWeaponExp;
	NewWeaponExp.EXPThresholds.Add(0.0f);
	NewWeaponExp.EXPThresholds.Add(200.0f);
	NewWeaponExp.EXPThresholds.Add(300.0f);
	// NewWeaponExp.EXPThresholds.Add(400.0f);
	WeaponExp.Add(EWeaponType::Pistol, NewWeaponExp);

	WeaponLevel.Add(EWeaponType::Pistol, 1);
	WeaponXP.Add(EWeaponType::Pistol, 0);
	/*WeaponXP.Add(EWeaponType::Pistol, 0);
	WeaponXP.Add(EWeaponType::Rifle, 0);
	WeaponXP.Add(EWeaponType::Shotgun, 0);
	
	WeaponLevel.Add(EWeaponType::Pistol, 1);
	WeaponLevel.Add(EWeaponType::Rifle, 1);
	WeaponLevel.Add(EWeaponType::Shotgun, 1);*/


	// static ConstructorHelpers::FClassFinder<ULyraInventoryItemDefinition> WeaponItemClass(TEXT("/Script/Engine.Blueprint'/Game/Weapons/Pistol/ID_Pistol_SSLv2.ID_Pistol_SSLv2'"));
	// if (WeaponItemClass.Succeeded())
	// {
	// 	WeaponItemDefinition = WeaponItemClass.Class;
	// }
	
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

void ASSCharacter::CheckLevelUp(EWeaponType WeaponType)
{
	if(WeaponExp[WeaponType].WeaponLevel==3) return;
	// if(!WeaponExp[WeaponType].EXPThresholds[WeaponExp[WeaponType].WeaponLevel]) return;

	if(WeaponExp[WeaponType].CurrentEXP>=WeaponExp[WeaponType].EXPThresholds[WeaponExp[WeaponType].WeaponLevel]
	&& WeaponExp[WeaponType].WeaponLevel<WeaponExp[WeaponType].EXPThresholds.Num())
	{
		WeaponExp[WeaponType].CurrentEXP-=WeaponExp[WeaponType].EXPThresholds[WeaponExp[WeaponType].WeaponLevel];
		WeaponExp[WeaponType].WeaponLevel++;
		OnLevelUp(WeaponType, WeaponExp[WeaponType].WeaponLevel);
		
		//OnXPChangedDelegate.Broadcast(WeaponExp[WeaponType].CurrentEXP);
		//OnLevelChangedDelegate.Broadcast(WeaponExp[WeaponType].WeaponLevel);
		return;
	}
	//OnXPChangedDelegate.Broadcast(WeaponExp[WeaponType].CurrentEXP);
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

void ASSCharacter::AddEXP(EWeaponType WeaponType, float EXP)
{
	// Add EXP
	if(WeaponType== EWeaponType::Healing) return;
	if(WeaponExp.Contains(WeaponType))
	{
		WeaponExp[WeaponType].CurrentEXP+=EXP;
	}
	else
	{
		FWeaponExperience NewWeaponExp;
		NewWeaponExp.EXPThresholds.Add(0.0f);
		NewWeaponExp.EXPThresholds.Add(200.0f);
		NewWeaponExp.EXPThresholds.Add(300.0f);
		// NewWeaponExp.EXPThresholds.Add(400.0f);
		WeaponExp.Add(WeaponType, NewWeaponExp);
	}
	CheckLevelUp(WeaponType);
}

float ASSCharacter::GetWeaponExp(EWeaponType WeaponType) const
{
	return WeaponExp.Contains(WeaponType) ? WeaponExp[WeaponType].CurrentEXP : 0.0f;
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

int32 ASSCharacter::GetLevel_Implementation(EWeaponType WeaponType) const
{
	return WeaponLevel[WeaponType];
}

int32 ASSCharacter::GetXP_Implementation(EWeaponType WeaponType) const
{
	return WeaponXP[WeaponType];
}


int32 ASSCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	return LevelUpInfo->FindLevelForXP(InXP);
}
void ASSCharacter::AddXP_Implementation(int32 InXP,EWeaponType WeaponType) 
{
	WeaponXP[WeaponType]+=InXP;
	OnXPChangedDelegate.Broadcast(WeaponXP[WeaponType]);
}

void ASSCharacter::AddWeaponLevel_Implementation(int32 InLevel, EWeaponType WeaponType)
{
	WeaponLevel[WeaponType]+=InLevel;
	OnLevelChangedDelegate.Broadcast(WeaponLevel[WeaponType]);
}

void ASSCharacter::ApplyXP(EWeaponType WeaponType)
{
	if(!WeaponLevel.Contains(WeaponType))
	{
		WeaponXP.Add(WeaponType,0);
		WeaponLevel.Add(WeaponType, 1);
		OnXPChangedDelegate.Broadcast(WeaponXP[WeaponType]);
		OnLevelChangedDelegate.Broadcast(WeaponLevel[WeaponType]);
		return;
	}
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


void ASSCharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponXPViewModel=NewObject<UMVVM_WeaponXP>(this, WeaponXPViewModelClass);
	WeaponXPViewModel->Initialize(this);
	
}
