// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Projectile.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = capsuleComponent;

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	baseMesh->SetupAttachment(capsuleComponent);

	turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	turretMesh->SetupAttachment(baseMesh);

	projectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Shoot Point"));
	projectileSpawnPoint->SetupAttachment(turretMesh);
}

void ABasePawn::HandleDestruction()
{
	if (deathParticleEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(this, deathParticleEffect, GetActorLocation(), GetActorRotation());
	}

	if (deathSound) {
		UGameplayStatics::PlaySoundAtLocation(this, deathSound, GetActorLocation());
	}

	if (DeathCameraShakeClass) {
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}

void ABasePawn::RotateTurret(FVector lookAtTarget)
{
	FVector toTarget = lookAtTarget - turretMesh->GetComponentLocation();
	FRotator lookAtRotation = FRotator(0.f, toTarget.Rotation().Yaw, 0.f);
	turretMesh->SetWorldRotation(
		FMath::RInterpTo(turretMesh->GetComponentRotation(), 
			lookAtRotation, 
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			turretSpeed));
}

void ABasePawn::Fire()
{
	FVector projectileLocation = projectileSpawnPoint->GetComponentLocation();
	FRotator projectileRotation = projectileSpawnPoint->GetComponentRotation();
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, projectileLocation, projectileRotation);
	projectile->SetOwner(this);
}
