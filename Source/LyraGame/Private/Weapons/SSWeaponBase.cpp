// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SSWeaponBase.h"

#include "Character/SSCharacter.h"

// Sets default values
ASSWeaponBase::ASSWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
	RootComponent= WeaponMesh_3p= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh_3P"));
	WeaponMesh_3p->CastShadow = true;
	WeaponMesh_3p->bCastHiddenShadow = true;
	
	WeaponMesh_1p = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh_1P"));
	WeaponMesh_1p->SetupAttachment(RootComponent);
	WeaponMesh_1p->CastShadow = false;
	WeaponMesh_1p->bCastHiddenShadow = false;
	WeaponMesh_1p->SetOnlyOwnerSee(true);	
	WeaponMesh_1p->SetOwnerNoSee(true);
	
}

// Called when the game starts or when spawned
void ASSWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* PawnOwner=Cast<APawn>(GetOwner());

	if (GetNetMode() != NM_DedicatedServer)
	{
		if (ASSCharacter* Character=Cast<ASSCharacter>(PawnOwner))
		{
			WeaponMesh_1p->AttachToComponent(Character->GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttackSocket);
			WeaponMesh_1p->SetRelativeRotation( FRotator(0.0f, -90.0f, 0.0f) );

			Character->SetCurrentWeapon(this);
			Character->OnPlayerEquippedNewWeapon(WeaponCameraOffset);
			
			if(Character->IsFirstPerson==true)
			{
				WeaponMesh_1p->SetOwnerNoSee(false);
				WeaponMesh_3p->SetOwnerNoSee(true);
			}
			else
			{
				WeaponMesh_1p->SetOwnerNoSee(true);
				WeaponMesh_3p->SetOwnerNoSee(false);
			}
			
		}
	}
}



USkeletalMeshComponent* ASSWeaponBase::GetProperWeaponMesh() const
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	ASSCharacter* Character= Cast<ASSCharacter>(PawnOwner);
	
	return PawnOwner->IsLocallyControlled() && Character->IsFirstPerson ? WeaponMesh_1p : WeaponMesh_3p;
}

