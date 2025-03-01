// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SSAnimInstance.h"

#include "Camera/LyraCameraComponent.h"
#include "Character/LyraCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/SSWeaponBase.h"
#include "Character/SSCharacter.h"

class ULyraCameraComponent;

USSAnimInstance::USSAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

#pragma region PROCEDUAL IK

void USSAnimInstance::OnNewWeaponEquipped(float NewWeaponOffset)
{
	SightOffset = NewWeaponOffset;

	FTimerHandle SyncWeapon_TimerHandle;
	
	GetWorld()->GetTimerManager().SetTimer(SyncWeapon_TimerHandle, this, &ThisClass::FinalizeWeaponSync, 0.55f, false);

}

void USSAnimInstance::SetIsAiming(bool bNewAiming)
{
	if (bIsAiming != bNewAiming)
	{
		bIsAiming= bNewAiming;
		bInterpAiming= true;
	}
}

void USSAnimInstance::OnAnimNotify_WeaponEquipComplete()
{
	FTimerHandle SyncWeapon_TimerHandle;
	
	GetWorld()->GetTimerManager().SetTimer(SyncWeapon_TimerHandle, this, &ThisClass::FinalizeWeaponSync, 0.1f, false);
	CanADS= true;
}

void USSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!SSCharacter)
	{
		SSCharacter = Cast<ASSCharacter>(GetOwningActor());
	}

	if(!SSCharacter) return;

	ULyraCharacterMovementComponent* CharMoveComp = CastChecked<ULyraCharacterMovementComponent>(SSCharacter->GetCharacterMovement());
	const FLyraCharacterGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;

	if(bInterpAiming)
	{
		InterpAiming(DeltaSeconds);
	}
}

void USSAnimInstance::SetRelativeHandTransform()
{
	if (!IsValid(SSCharacter)) { return; }

	if (!SSCharacter->IsLocallyControlled()) { return; }

	if (SSCharacter->GetNetMode() != NM_DedicatedServer)
	{
		if (!SSCharacter->GetCurrentWeapon() || !IsValid(SSCharacter->GetCurrentWeapon()->GetWeaponMesh1P())) { return; }

		const FTransform OpticTransform = SSCharacter->GetCurrentWeapon()->GetWeaponMesh1P()->GetSocketTransform(FName("S_IronSight"));
		const FTransform MeshTransform = SSCharacter->GetFirstPersonMesh()->GetSocketTransform(FName("hand_r"));

		RelativeHandTransform = UKismetMathLibrary::MakeRelativeTransform(OpticTransform, MeshTransform);

	}
}

void USSAnimInstance::SetSightTransform()
{
	if (!IsValid(SSCharacter)) { return; }

	if (!SSCharacter->IsLocallyControlled()) { return; }

	if (SSCharacter->GetNetMode() != NM_DedicatedServer)
	{
		const FTransform CameraTransform = SSCharacter->GetCameraComponent()->GetComponentTransform();
		const FTransform MeshTransform = SSCharacter->GetFirstPersonMesh()->GetComponentTransform();
	

		SightTransForm =UKismetMathLibrary::MakeRelativeTransform(CameraTransform, MeshTransform);
		SightTransForm.SetLocation(SightTransForm.GetLocation()+ SightTransForm.GetRotation().Vector() * SightOffset);
		
	}
}

void USSAnimInstance::InterpAiming(float DeltaSeconds)
{
	AimAlpha= FMath::FInterpTo(AimAlpha, bIsAiming, DeltaSeconds, 15.0f);

	if (AimAlpha >= 1.0f || AimAlpha<=0.0f)
	{
		bInterpAiming = false;
	}
}

void USSAnimInstance::FinalizeWeaponSync()
{
	SetSightTransform();
	SetRelativeHandTransform();
}

#pragma endregion	
