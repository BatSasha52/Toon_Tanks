// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	//RootComponent = capsuleComponent;

	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = projectileMesh;
	

	trailParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke trail"));
	trailParticleEffect->SetupAttachment(RootComponent);

	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	movementComponent->MaxSpeed = maxSpeed;
	movementComponent->InitialSpeed = initialSpeed;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	projectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (launchSound) {
		UGameplayStatics::PlaySoundAtLocation(this, launchSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	AActor* myOwner = GetOwner();
	if (myOwner == nullptr) {
		Destroy();
		return;
	}
	
	AController* myOwnerInstigator = myOwner->GetInstigatorController();
	UClass* damageTypeClass = UDamageType::StaticClass();
	if (myOwnerInstigator && otherActor != this && otherActor != myOwner) {
		UGameplayStatics::ApplyDamage(otherActor, damage, myOwnerInstigator, this, damageTypeClass);

		if (hitParticleEffect) {
			UGameplayStatics::SpawnEmitterAtLocation(this, hitParticleEffect, GetActorLocation(), GetActorRotation());
		}

		if (hitSound) {
		UGameplayStatics::PlaySoundAtLocation(this, hitSound, GetActorLocation());
		}

		if (HitCameraShakeClass) {
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
	}

	Destroy();
}

