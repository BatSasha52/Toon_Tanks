// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksPlayerController.h"
#include "Tank.h"
#include "Tower.h"

void AToonTanksGameMode::ActorDied(AActor* deadActor)
{
	if (deadActor == tank) {
		tank->HandleDestruction();
		if (toonTanksPlayerController) {
			toonTanksPlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
	}
	else if (ATower* destroyedTower = Cast<ATower>(deadActor)) {
		destroyedTower->HandleDestruction();
		towersCount--;

		if (towersCount <= 0) {
			GameOver(true);
		}
	}
}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
	towersCount = GetTowersCount();
	tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	toonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (toonTanksPlayerController) {
		toonTanksPlayerController->SetPlayerEnabledState(false);

		FTimerHandle playerEnableTimerHandle;
		FTimerDelegate playerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			toonTanksPlayerController,
			&AToonTanksPlayerController::SetPlayerEnabledState,
			true
		);
		GetWorldTimerManager().SetTimer(playerEnableTimerHandle,
			playerEnableTimerDelegate,
			startDelay,
			false
		);
	}
}

int32 AToonTanksGameMode::GetTowersCount()
{
	TArray<AActor*> towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), towers);
	return towers.Num();

}