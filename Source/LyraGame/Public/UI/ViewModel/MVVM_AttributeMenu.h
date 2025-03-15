// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_AttributeMenu.generated.h"

class ASSPlayerState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignautre, int32, NewValue);

/**
 * 
 */
UCLASS()
class LYRAGAME_API UMVVM_AttributeMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void Initialize(ASSPlayerState* InSSPlayerState);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignautre StatPointsChangeDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="ture"))
	int32 StatPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="ture"))
	int32 DamageStat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="ture"))
	int32 HealthStat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="ture"))
	int32 SpeedStat;

	UFUNCTION()
	void OnStatPointsChanged(int32 InStatPoints);

	void SetStatPoints(int32 InStatPoints);
	int32 GetStatPoints() const { return StatPoints; };

	void SetDamageStat(int32 InStat);
	int32 GetDamageStat() const { return DamageStat; };
	void SetHealthStat(int32 InStat);
	int32 GetHealthStat() const { return HealthStat; };
	void SetSpeedStat(int32 InStat);
	int32 GetSpeedStat() const { return SpeedStat; };

	UFUNCTION(BlueprintCallable)
	void UpgradeStat(FString AttributeName);

	
	UPROPERTY()
	ASSPlayerState* SSPlayerState;
	
};


