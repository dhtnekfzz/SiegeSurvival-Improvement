
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/LyraWeaponInstance.h"
#include "SSWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()	
class LYRAGAME_API USSWeaponInstance : public ULyraWeaponInstance
{
	GENERATED_BODY()

public:
	USSWeaponInstance(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FLyraAnimLayerSelectionSet EquippedAnimSet_3P;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FLyraAnimLayerSelectionSet UnequippedAnimSet_3P;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraAnimLayerSelectionSet EquippedAnimSet_1P;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraAnimLayerSelectionSet UnequippedAnimSet_1P;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	UAnimMontage* EquippedAnim_3P;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	UAnimMontage* UnequippedAnim_3P;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	UAnimMontage* EquippedAnim_1P;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	UAnimMontage* UnequippedAnim_1P;
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Animation)
	virtual UPARAM(DisplayName="Anim Layer 3P") TSubclassOf<UAnimInstance> BestPickAnimLayer(bool bEquipped, const FGameplayTagContainer & CosmeticTags, TSubclassOf<UAnimInstance>& AnimLayer_1P) const;

};
