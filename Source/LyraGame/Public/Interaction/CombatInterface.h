// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

enum class EWeaponType : uint8;
struct FGameplayTag;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LYRAGAME_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetWeaponLevel(EWeaponType WeaponType) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetWeaponXP(EWeaponType WeaponType) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 FindWeaponLevelForXP(int32 InXP) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddWeaponXP(int32 InXP,EWeaponType WeaponType);

	UFUNCTION(BlueprintNativeEvent)
	void AddWeaponLevel(int32 InLevel,EWeaponType WeaponType);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerXP() const;

	UFUNCTION(BlueprintNativeEvent)
	void AddPlayerXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void AddPlayerLevel(int32 InLevel);

	UFUNCTION(BlueprintNativeEvent)
	int32 FindPlayerLevelForXP(int32 InXP) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddStatPoints(int32 InStatPoints);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetStatPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetStatPointsReward(int32 Level) const;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);
};
