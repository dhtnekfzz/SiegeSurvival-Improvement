// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SSProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASSProjectile::ASSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Sphere=CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);

	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed=550.f;
	ProjectileMovement->MaxSpeed=550.f;
	ProjectileMovement->ProjectileGravityScale=0.f;

}

// Called when the game starts or when spawned
void ASSProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);
	LoopingSoundComponent=UGameplayStatics::SpawnSoundAttached(LoopingSound, RootComponent);
}

void ASSProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if(LoopingSoundComponent) LoopingSoundComponent->Stop();
	bHit=true;
}

void ASSProjectile::Destroyed()
{
	Super::Destroyed();
}





