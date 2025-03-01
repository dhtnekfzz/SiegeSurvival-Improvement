// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/LyraAnimInstance.h"
#include "Character/SSCharacter.h"
#include "SSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API USSAnimInstance : public ULyraAnimInstance
{
	GENERATED_BODY()

public:

	USSAnimInstance(const FObjectInitializer& ObjectInitializer);

#pragma region PROCEDUAL IK
	
	UPROPERTY(BlueprintReadOnly, Category = "Procedual IK")
	ASSCharacter* SSCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Procedual IK")
	FTransform RelativeHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Procedual IK")
	FTransform SightTransForm;

	UPROPERTY(BlueprintReadOnly, Category = "Procedual IK")
	float AimAlpha = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Procedual IK")
    bool CanADS= false;

	UFUNCTION(BlueprintCallable, Category = "Procedual IK")
	void OnNewWeaponEquipped(float NewWeaponOffset);

	UFUNCTION(BlueprintCallable, Category = "Procedual IK")
	void SetIsAiming(bool bNewAiming);

	UFUNCTION(BlueprintCallable, Category = "Procedual IK")
	void OnAnimNotify_WeaponEquipComplete();

protected:
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void SetRelativeHandTransform();
	void SetSightTransform();
	void InterpAiming(float DeltaSeconds);


private:

	float SightOffset = 30.0f;

	void FinalizeWeaponSync();

	bool bInterpAiming = false;

	bool bIsAiming = false;


#pragma endregion
	
	

};
