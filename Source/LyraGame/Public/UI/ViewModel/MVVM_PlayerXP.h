// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_PlayerXP.generated.h"

class ASSPlayerState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerXPPercentChnaged, float, NewValue);

/**
 * 
 */
UCLASS()
class LYRAGAME_API UMVVM_PlayerXP : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void Initialize(ASSPlayerState* SSPlayerState);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerXPPercentChnaged OnXPPercentChangedDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="ture"))
	int32 PlayerXP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="ture"))
	int32 PlayerLevel;

	UFUNCTION()
	void OnPlayerXPChanged(int32 NewXP);

	UFUNCTION()
	void OnPlayerLevelChanged(int32 NewLevel);

	void SetPlayerXP(int32 InXP);
	void SetPlayerLevel(int32 InLevel);

	int32 GetPlayerXP() const { return PlayerXP; }
	int32 GetPlayerLevel() const { return PlayerLevel; }

	UPROPERTY()
	ASSPlayerState* SSPlayerState;

	
};
