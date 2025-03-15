// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayEffect.h"
#include "CharacterClassInfo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECharacterClass
{
	Gideon,
	Phase,
	Revenant,
	Grux,
	Crunch,
	Khaimera,
	Wraith
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, Category="Class Defaults")
	FScalableFloat XPReward=FScalableFloat();
};


/**
 * 
 */
UCLASS()
class LYRAGAME_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	FCharacterClassDefaultInfo GetDefaultInfo(ECharacterClass CharacterClass);
};
