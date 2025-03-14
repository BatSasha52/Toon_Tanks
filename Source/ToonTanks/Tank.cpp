// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
	
}

void ATank::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	playerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	playerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

	playerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (tankPlayerController)
	{
		FHitResult hitResult;
		tankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false,
			hitResult);

		RotateTurret(hitResult.ImpactPoint);
	}

}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	tankPlayerController = Cast<APlayerController>(GetController());
}

void ATank::Move(float value)
{
	//UE_LOG(LogTemp, Display, TEXT("%f"), value);

	FVector deltaLocation(0.f);
	deltaLocation.X = value * movementSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(deltaLocation, true);
}

void ATank::Turn(float value)
{
	FRotator deltaRotation = FRotator::ZeroRotator;
	deltaRotation.Yaw = value * rotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(deltaRotation, true);
}

void ATank::Fire()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastFireTime >= FireRate)
	{
		Super::Fire();
		LastFireTime = CurrentTime;
	}
}