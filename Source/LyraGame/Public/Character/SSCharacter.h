// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LyraCharacter.h"
#include "Interaction/CombatInterface.h"
#include "SSCharacter.generated.h"

class UMVVM_WeaponXP;
class ULevelUpInfo;
class ULyraInventoryItemDefinition;
enum class EWeaponType : uint8;
class ULyraCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32, NewValue);

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponItem : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category="Weapon|Level")
	int32 WeaponLevel;

	UPROPERTY(EditAnywhere, Category="Weapon|Level")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, Category="Weapon|Level")
	TSubclassOf<ULyraInventoryItemDefinition> WeaponItemClass;
};

USTRUCT(BlueprintType)
struct FWeaponExperience : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 현재 경험치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Level")
	float CurrentEXP = 0.0f;

	// 현재 레벨
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Level")
	int32 WeaponLevel = 1;

	// 레벨업에 필요한 경험치 임계값
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Level")
	TArray<float> EXPThresholds;
};

UCLASS()
class LYRAGAME_API ASSCharacter : public ALyraCharacter, public ICombatInterface
{
	GENERATED_BODY()
	
public:
	
	ASSCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChanged OnXPChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChanged OnLevelChangedDelegate;
	
	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	float GetSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	FORCEINLINE USkeletalMeshComponent* GetFirstPersonMesh() const { return Mesh1p; }

	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	FORCEINLINE ULyraCameraComponent* GetCameraComponent() const {  return CameraComponent; }
	
	UFUNCTION(BlueprintPure, Category = "SS|Character")
	FORCEINLINE class ASSWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }
	
	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	void SetCurrentWeapon(ASSWeaponBase* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	void OnPlayerEquippedNewWeapon(float WeaponOffset);

	UFUNCTION(BlueprintCallable, Category = "SS|Character")
	void SetIsFirstPerson();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Level")
	void CheckLevelUp(EWeaponType WeaponType);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon|Level")
	void OnLevelUp(EWeaponType WeaponType, int32 Level);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Level")
	void AddEXP(EWeaponType WeaponType, float EXP);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Level")
	float GetWeaponExp(EWeaponType WeaponType) const;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Level")
	FORCEINLINE int32 GetWeaponLevel(EWeaponType WeaponType) const { return WeaponExp.Contains(WeaponType) ? WeaponExp[WeaponType].WeaponLevel : 0; }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure=false, Category = "Weapon|Level")
	bool WeaponLevelUp(TSubclassOf<ULyraInventoryItemDefinition> WeaponItemClass, APawn* ReceivingPawn, EWeaponType WeaponType);

	/* Combat Interface */
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetLevel_Implementation(EWeaponType WeaponType) const override;
	virtual int32 GetXP_Implementation(EWeaponType WeaponType) const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual void AddXP_Implementation(int32 InXP, EWeaponType WeaponType) override;
	virtual void AddWeaponLevel_Implementation(int32 InLevel, EWeaponType WeaponType) override;
	/* end Combat Interface */

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<EWeaponType, int32> WeaponLevel;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TMap<EWeaponType, int32> WeaponXP;

	void ApplyXP(EWeaponType WeaponType);
	
	//Weapon
	UPROPERTY()
	class ASSWeaponBase* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "SS|Character")
	bool IsFirstPerson = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Level")
	TMap<EWeaponType, FWeaponExperience> WeaponExp;

	UPROPERTY(EditAnywhere,  Category="Weapon|Level")
	TSubclassOf<ULyraInventoryItemDefinition> WeaponItemDefinition;

	UPROPERTY(EditAnywhere,  Category="Weapon|Level")
	TObjectPtr<UDataTable> WeaponItemTable;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftHandSocketName;

	UFUNCTION()
	UMVVM_WeaponXP* GetWeaponXPViewModel() const { return WeaponXPViewModel; }
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SS|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh1p;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_WeaponXP> WeaponXPViewModelClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMVVM_WeaponXP> WeaponXPViewModel;
};

