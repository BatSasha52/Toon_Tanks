// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h" 
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTankInRange()){
		RotateTurret(tank->GetActorLocation());
	}
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	fireRate = UKismetMathLibrary::RandomFloatInRange(2.f, 4.f);

	GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &ATower::CheckFireCondition, fireRate, true);
}

bool ATower::IsTankInRange()
{
	if (tank) {
		float distance = FVector::Dist(GetActorLocation(), tank->GetActorLocation());
		return distance <= fireRange;
	}
	else return false;
}

void ATower::CheckFireCondition()
{
	if (tank == nullptr) {
		return;
	}

	if (IsTankInRange() && tank->bAlive) {
		Fire();
	}
}
