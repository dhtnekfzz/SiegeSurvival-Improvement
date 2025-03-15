// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/LyraPlayerState.h"
#include "SSPlayerState.generated.h"

enum class EWeaponType : uint8;
class UMVVM_WeaponXP;
class UMVVM_AttributeMenu;
class UMVVM_PlayerXP;
class ULevelUpInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32, NewValue);

UCLASS()
class LYRAGAME_API ASSPlayerState : public ALyraPlayerState
{
	GENERATED_BODY()

public:
	ASSPlayerState();

	virtual void BeginPlay() override;

	FOnPlayerStatChanged OnPlayerXPChangedDelegate;
	FOnPlayerStatChanged OnPlayerLevelChangedDelegate;
	FOnPlayerStatChanged OnStatPointsChangedDelegate;
	FOnPlayerStatChanged OnWeaponXPChangedDelegate;
	FOnPlayerStatChanged OnWeaponLevelChangedDelegate;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> WeaponLevelUpInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_PlayerXP> PlayerXPViewModelClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMVVM_PlayerXP> PlayerXPViewModel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_AttributeMenu> AttributeMenuViewModelClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMVVM_AttributeMenu> AttributeMenuViewModel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_WeaponXP> WeaponXPViewModelClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMVVM_WeaponXP> WeaponXPViewModel;
	
	UFUNCTION()
	UMVVM_PlayerXP* GetPlayerXPViewModel() const { return PlayerXPViewModel; }
	UFUNCTION()
	UMVVM_AttributeMenu* GetAttributeMenuViewModel() const { return AttributeMenuViewModel; }
	UFUNCTION()
	UMVVM_WeaponXP* GetWeaponXPViewModel() const { return WeaponXPViewModel; }

	FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; };
	FORCEINLINE int32 GetPlayerXP() const { return PlayerXP;};
	FORCEINLINE int32 GetStatPoints() const { return StatPoints; };

	void AddToPlayerXP(int32 InXP);
	void AddToPlayerLevel(int32 InLevel);
	void AddToStatPoints(int32 InStatPoints);
	void SetPlayerXP(int32 InXP);
	void SetPlayerLevel(int32 InLevel);
	void SetStatPoints(int32 InStatPoints);

	FORCEINLINE int32 GetWeaponLevel(EWeaponType Weapon) const { return WeaponLevel[Weapon]; };
	FORCEINLINE int32 GetWeaponXP(EWeaponType Weapon) const { return WeaponXP[Weapon];};
	
	void AddToWeaponXP(int32 InXP, EWeaponType Weapon);
	void AddToWeaponLevel(int32 InLevel, EWeaponType Weapon);
	void SetWeaponXP(int32 InXP, EWeaponType Weapon);
	void SetWeaponLevel(int32 InLevel, EWeaponType Weapon);


private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 PlayerLevel=1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 PlayerXP=0;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_StatPoints)
	int32 StatPoints=0;

	UPROPERTY(VisibleAnywhere)
	TMap<EWeaponType, int32> WeaponLevel;

	UPROPERTY(VisibleAnywhere)
	TMap<EWeaponType, int32> WeaponXP;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_StatPoints(int32 OldStatPoints);

};


