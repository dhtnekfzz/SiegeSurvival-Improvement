// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSWeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	Healing UMETA(DisplayName = "Healing"),
};


UCLASS()
class LYRAGAME_API ASSWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASSWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintPure, Category = "Weapon")
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh1P() const { return WeaponMesh_1p; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh3P() const { return WeaponMesh_3p; }

	UFUNCTION(BlueprintPure, Category = "WeaponType")
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

public:
	UPROPERTY(EditAnywhere, Category = "WeaponType")
	EWeaponType WeaponType;

protected:


protected:
	
	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh_1p;

	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh_3p;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	FName AttackSocket= "weapon_r";

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	float WeaponCameraOffset = 30.0f;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	USkeletalMeshComponent* GetProperWeaponMesh() const;

};
