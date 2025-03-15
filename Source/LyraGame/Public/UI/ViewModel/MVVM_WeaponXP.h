// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_WeaponXP.generated.h"

class ASSPlayerState;
class ASSCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXPPercentChnaged, float, NewValue);
/**
 * 
 */
UCLASS()
class LYRAGAME_API UMVVM_WeaponXP : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void Initialize(ASSPlayerState* InPlayerState);

	UPROPERTY(BlueprintAssignable)
	FOnXPPercentChnaged OnXPPercentChangedDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="ture"))
	int32 WeaponXP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="ture"))
	int32 WeaponLevel=1;

	UFUNCTION()
	void OnWeaponXPChanged(int32 NewXP);

	UFUNCTION()
	void OnWeaponLevelChanged(int32 NewLevel);

	void SetWeaponXP(int32 InXP);
	void SetWeaponLevel(int32 InLevel);

	int32 GetWeaponXP() const { return WeaponXP; }
	int32 GetWeaponLevel() const { return WeaponLevel; }

	UPROPERTY()
	ASSPlayerState* SSPlayerState;
};
