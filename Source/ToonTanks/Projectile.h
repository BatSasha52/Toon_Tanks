// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* capsuleComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* projectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UProjectileMovementComponent* movementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UParticleSystemComponent* trailParticleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float initialSpeed{ 1.f };

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float maxSpeed{ 5.f };

	UPROPERTY(EditAnywhere, Category = "Combat")
	float damage{ 40.f };

	UFUNCTION()
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* hitParticleEffect;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* launchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;
};
