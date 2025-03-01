// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "SSProjectileSpell.generated.h"

class ASSProjectile;
/**
 * 
 */
UCLASS()
class LYRAGAME_API USSProjectileSpell : public ULyraGameplayAbility
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASSProjectile> ProjectileClass;
	
};
