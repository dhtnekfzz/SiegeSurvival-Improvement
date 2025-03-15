// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/LyraGameplayEffectContext.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "SSAttributeSet.generated.h"

enum class EWeaponType : uint8;

UCLASS()
class LYRAGAME_API USSAttributeSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:

	USSAttributeSet();
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "XP")
	FGameplayAttributeData RifleXP;
	ATTRIBUTE_ACCESSORS(USSAttributeSet, RifleXP)

	UPROPERTY(BlueprintReadOnly, Category = "XP")
	FGameplayAttributeData ShotgunXP;
	ATTRIBUTE_ACCESSORS(USSAttributeSet, ShotgunXP)

	UPROPERTY(BlueprintReadOnly, Category = "XP")
	FGameplayAttributeData PistolXP;
	ATTRIBUTE_ACCESSORS(USSAttributeSet, PistolXP)

	UPROPERTY(BlueprintReadOnly, Category="XP")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(USSAttributeSet, IncomingXP);

private:
	void HandleIncomingXP(int32 XP, EWeaponType WeaponTyp, const FGameplayEffectModCallbackData& Data);
	void HandleIncomingPlayerXP(int32 XP, const FGameplayEffectModCallbackData& Data);
};
