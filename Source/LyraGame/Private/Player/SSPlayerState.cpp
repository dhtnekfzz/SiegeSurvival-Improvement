// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SSPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "UI/ViewModel/MVVM_AttributeMenu.h"
#include "UI/ViewModel/MVVM_PlayerXP.h"
#include "UI/ViewModel/MVVM_WeaponXP.h"
#include "Weapons/SSWeaponBase.h"

ASSPlayerState::ASSPlayerState()
{
	WeaponLevel.Add(EWeaponType::Pistol);
	WeaponLevel.Add(EWeaponType::Rifle);
	WeaponLevel.Add(EWeaponType::Shotgun);

	WeaponXP.Add(EWeaponType::Pistol);
	WeaponXP.Add(EWeaponType::Rifle);
	WeaponXP.Add(EWeaponType::Shotgun);

	WeaponLevel[EWeaponType::Pistol]=1;
	WeaponLevel[EWeaponType::Rifle]=1;
	WeaponLevel[EWeaponType::Shotgun]=1;

	
	
}

void ASSPlayerState::BeginPlay()
{
	Super::BeginPlay();
	PlayerXPViewModel=NewObject<UMVVM_PlayerXP>(this, PlayerXPViewModelClass);
	PlayerXPViewModel->Initialize(this);

	AttributeMenuViewModel=NewObject<UMVVM_AttributeMenu>(this,AttributeMenuViewModelClass);
	AttributeMenuViewModel->Initialize(this);

	WeaponXPViewModel=NewObject<UMVVM_WeaponXP>(this,WeaponXPViewModelClass);
	WeaponXPViewModel->Initialize(this);
}

void ASSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASSPlayerState, PlayerLevel);
	DOREPLIFETIME(ASSPlayerState, PlayerXP);
	DOREPLIFETIME(ASSPlayerState, StatPoints);
}

void ASSPlayerState::AddToPlayerXP(int32 InXP)
{
	PlayerXP+=InXP;
	OnPlayerXPChangedDelegate.Broadcast(PlayerXP);
}


void ASSPlayerState::AddToPlayerLevel(int32 InLevel)
{
	PlayerLevel+=InLevel;
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

void ASSPlayerState::AddToStatPoints(int32 InStatPoints)
{
	StatPoints+=InStatPoints;
	OnStatPointsChangedDelegate.Broadcast(StatPoints);
}

void ASSPlayerState::SetPlayerXP(int32 InXP)
{
	PlayerXP=InXP;
	OnPlayerXPChangedDelegate.Broadcast(PlayerXP);
}

void ASSPlayerState::SetPlayerLevel(int32 InLevel)
{
	PlayerLevel=InLevel;
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

void ASSPlayerState::SetStatPoints(int32 InStatPoints)
{
	StatPoints=InStatPoints;
	OnStatPointsChangedDelegate.Broadcast(StatPoints);
}

void ASSPlayerState::AddToWeaponXP(int32 InXP, EWeaponType Weapon)
{
	WeaponXP[Weapon]+=InXP;
	OnWeaponXPChangedDelegate.Broadcast(WeaponXP[Weapon]);
}

void ASSPlayerState::AddToWeaponLevel(int32 InLevel, EWeaponType Weapon)
{
	WeaponLevel[Weapon]+=InLevel;
	OnWeaponLevelChangedDelegate.Broadcast(WeaponLevel[Weapon]);
}

void ASSPlayerState::SetWeaponXP(int32 InXP, EWeaponType Weapon)
{
	WeaponXP[Weapon]=InXP;
	OnWeaponXPChangedDelegate.Broadcast(WeaponXP[Weapon]);
}

void ASSPlayerState::SetWeaponLevel(int32 InLevel, EWeaponType Weapon)
{
	WeaponLevel[Weapon]=InLevel;
	OnWeaponLevelChangedDelegate.Broadcast(WeaponLevel[Weapon]);
}

void ASSPlayerState::OnRep_Level(int32 OldLevel)
{
	
}

void ASSPlayerState::OnRep_XP(int32 OldXP)
{
}

void ASSPlayerState::OnRep_StatPoints(int32 OldStatPoints)
{
}
