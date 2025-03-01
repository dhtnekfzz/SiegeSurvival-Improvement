// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SSProjectileSpell.h"
#include "Actor/SSProjectile.h"
#include "Interaction/CombatInterface.h"

void USSProjectileSpell::SpawnProjectile(const FGameplayTag& SocketTag,
                                         bool bOverridePitch, float PitchOverride)
{
	ICombatInterface* CombatInterface=Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if(CombatInterface)
	{
		const FVector SocketLocation=ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
		FRotator Rotation=GetAvatarActorFromActorInfo()->GetActorForwardVector().Rotation();
		DrawDebugSphere(GetWorld(), SocketLocation, 20.f, 10, FColor::Red,  true, 10);
		

		if(bOverridePitch)
		{
			Rotation.Pitch=PitchOverride;
		}

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		ASSProjectile* Projectile=GetWorld()->SpawnActorDeferred<ASSProjectile>(
			ProjectileClass, SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->SetOwner(GetAvatarActorFromActorInfo());
		Projectile->FinishSpawning(SpawnTransform);
	}
}
